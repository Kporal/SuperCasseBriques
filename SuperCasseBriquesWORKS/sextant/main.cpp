#include <hal/multiboot.h>

#include <sextant/interruptions/idt.h>
#include <sextant/interruptions/irq.h>
#include <sextant/interruptions/handler/handler_tic.h>
#include <sextant/interruptions/handler/handler_clavier.h>

#include <drivers/PortSerie.h>
#include <drivers/timer.h>
#include <drivers/Trame.h>

#include <sextant/memoire/Memoire.h>
#include <sextant/memoire/segmentation/gdt.h>
#include <sextant/memoire/MemoirePion.h>
#include <sextant/memoire/pagination/MemoirePhysique.h>
#include <sextant/memoire/pagination/Pagination.h>

#include <sextant/ordonnancements/cpu_context.h>
#include <sextant/ordonnancements/preemptif/thread.h>
#include <sextant/types.h>
#include <sextant/Synchronisation/Spinlock/Spinlock.h>

extern "C" void __cxa_pure_virtual() { while (1); }

// Souris
#include <sextant/interruptions/handler/mouse_handler.h>
#include <drivers/Souris.h>

// Ecran Virtuel
#include <drivers/EcranV.h>
#include <drivers/EcranP.h>

// Clavier Virtuel
#include <drivers/ClavierP.h>
#include <drivers/ClavierV.h>

// Son
#include <drivers/Son.h>

// MultiFenetrage
#include <drivers/EcranPMF.h>

// Shell (Application)
#include <Applications/Shell/Shell.h>

extern char __e_kernel,__b_kernel, __b_data, __e_data,  __b_stack, __e_load ;
int i;

extern vaddr_t bootstrap_stack_bottom; //Adresse de dÃ©but de la pile d'exÃ©cution
extern size_t bootstrap_stack_size;//Taille de la pile d'exÃ©cution
unsigned long address; // TODO first assignement of this variable (required for Sextant_init)

Timer timer;

EcranV ecranV[6];
EcranP *ecranP;

Souris mouse;

Memoire *InterfaceMemoire;


extern Semaphore *semClavierV[MAXCONSOLE]; // dans ClavierP.cpp
extern char buffeur[MAXCONSOLE][TAILLE_BUF_CLAVIER]; // dans ClavierP.cpp

ClavierP *clavierPhysique;
ClavierV *clavierVirtuel[MAXCONSOLE];

Semaphore block_me;

void TicTacToc(int i){
	int position;
	volatile caseEcran *Video = ecranP->getOrigine();

	ticTac(0);

	// Raffaraichir l'ecran

	ecranP->raffraichir();

	// Afficher le curseur

	mouse.bouger(ecranP->getEcranActif());
	position = mouse.y * COLONNES + mouse.x; // position lineaire relative
	Video[position].caractere = '>';
	Video[position].couleurs = BLANC;
	if ((mouse.clicD)||(mouse.clicG)||(mouse.clicM))
		Video[position].couleurs = ROUGE;

	// Appeller l'ordonanceur
	sched_clk(i);
}


void Sextant_Init(){


	idt_setup();
	irq_setup();

	//Initialisation de la frequence de l'horloge
	timer.i8254_set_frequency(1000);
	asm volatile("sti\n");//Autorise les interruptions

	// Initialisation de la mŽmoire

	multiboot_info_t* mbi;
	mbi = (multiboot_info_t*)address;

	gdt_setup();

	InterfaceMemoire=Memoire::nouveau();

	InterfaceMemoire->mem_setup(& __e_kernel,(mbi->mem_upper<<10) + (1<<20),(EcranV*)&ecranV[0]);

	thread_subsystem_setup(bootstrap_stack_bottom,bootstrap_stack_size);

	// Initialisation de l'ordo
	sched_subsystem_setup();
	irq_set_routine(IRQ_TIMER, TicTacToc);

	// Initialisation de la souris sur l'ecran virtuel 0
	mouse.initialiser(&(ecranV[0]));

	// Innitialisation de l'Žcran physique
	ecranP = new EcranP((EcranV *)ecranV);

	// Initialisation des claviers Physiques et Virtuels.
	clavierPhysique = new ClavierP(&(ecranV[1]));
	clavierPhysique->start("Keyboard");

	for (int i=0;i<MAXCONSOLE;i++)
		clavierVirtuel[i] = new ClavierV(semClavierV[i],(char*)&(buffeur[i]));

	// Initialisation du handler clavier
	clavier_install();
}

extern "C" void Sextant_main(unsigned long magic, unsigned long addr){

	Sextant_Init();

	ecranV[0].afficherMot("CONSOLE 1\n");
	ecranV[0].miniprintf("Bienvenue dans sextant l'OS de l'EMN\nhttp://sextant.menaud.fr/");
	ecranV[0].sautDeLigne();
	ecranV[0].afficherCaractere(BLANC,ROUGE,0xB0);
	ecranV[0].afficherCaractere(BLANC,ROUGE,0xB1);
	ecranV[0].afficherCaractere(BLANC,ROUGE,0xB2);
	ecranV[0].sautDeLigne();
	ecranV[0].afficherMot("Cette OS minimaliste a ete developpe 'from scratch' par les etudiants dans le cadre de l'UV Systeme et Reseaux", ROUGE);
	ecranV[0].sautDeLigne();
	ecranV[0].sautDeLigne();
	ecranV[0].afficherMot("D'un point de vue utilisation, il pourrait etre comparer a un mini DOS (ancetre de windows)", ROUGE);
	ecranV[0].afficherMot("D'un point de vue technique, il integre plusieurs concepts avances comme par exemple l'ordonnancement preemptif\n", ROUGE);

	ecranV[1].afficherMot("CONSOLE 1\n");
	ecranV[2].afficherMot("CONSOLE 2\n");
	ecranV[3].afficherMot("CONSOLE 3\n");

	Shell *init = new Shell (ecranV, clavierVirtuel, 3);
}
