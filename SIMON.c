/* programme PARTIEL CAUDRON Vincent */

#include "stm32l1xx_nucleo.h"

 void enable_interrupt_ext(void);
 void init_TIM3(void);
 void init_PWM(void);
 void init_MOTEUR_EXT(void);
 void init_led(void);
 void init_BP(void);
 void wait(uint32_t tmp);
 void affiche(uint32_t num);
 void affiche_sequence(uint32_t num);
 void introduction(void);
 void suivant(void);
 void erreur(void);
 void init_BIP(void);
 void init_BIP_TIM3(void);
 
 void init_SPI(void);
 void SPI_Write(uint8_t add,uint8_t data);
 void SendData_SPI(uint8_t add);
 void Set_CS (void);
 void Reset_CS (void);
 void Afficheur(void);

 uint32_t compt;
 uint32_t rep;
 char sequence[40];
 uint32_t wait_tmp;
 uint8_t randNumber;

 int main()
{ 
 init_led();
 init_BP();
 enable_interrupt_ext();
 init_TIM3();
 init_PWM();  
 init_MOTEUR_EXT();
 init_BIP();
 init_BIP_TIM3();
  
 init_SPI();
 
 //Commandes d'initialisation pour MAX
  SPI_Write(0x0C,0x00); //Eteindre MAX
  SPI_Write(0x0C,0x01);  //Mode Normal
  SPI_Write(0x0A,0x0A); //Luminosité
  SPI_Write(0x09,0x0F); //Decode Mode car 4 digits
  SPI_Write(0x0B,0x03); // Scan Limit
  SPI_Write(0x0F,0x00); //Display test
  
  //Initialisation des digits à 0
  SPI_Write(0x01,0x00);
  SPI_Write(0x02,0x00);
  SPI_Write(0x03,0x00);
  SPI_Write(0x04,0x00);
 
 
 wait_tmp = 1000000;
 compt = 0;
 rep = 0;
  
 introduction();
 affiche_sequence(1);
  
  while(1)
  {  
    //Boucle
    /*randNumber = (uint8_t) rand();
    randNumber = (randNumber % 4) + 1 ; 
    if(randNumber == 1)
    {
      TIM3->PSC = 200;
      TIM3->CCER |= (1<<4);
      wait(wait_tmp);
      TIM3->CCER &= ~(1<<4);
    }
    if(randNumber == 2)
    {
      TIM3->PSC = 400;
      TIM3->CCER |= (1<<4);
      wait(wait_tmp);
      TIM3->CCER &= ~(1<<4);
    }
    if(randNumber == 3)
    {
      TIM3->PSC = 800;
      TIM3->CCER |= (1<<4);
      wait(wait_tmp);
      TIM3->CCER &= ~(1<<4);
    }
    if(randNumber == 4)
    {
      TIM3->PSC = 1000;
      TIM3->CCER |= (1<<4);
      wait(wait_tmp);
      TIM3->CCER &= ~(1<<4);
    }*/
  }
}

void enable_interrupt_ext(void)
{
  NVIC->ISER[1] |= (1<<8); //vecteur d'interuption
  NVIC->ISER[0] |= (1<<23);
  
  RCC->APB2ENR |= (1<<0); //Reset clock
  
  EXTI->IMR |= (1<<11); // activer interuption
  EXTI->IMR |= (1<<12);
  EXTI->IMR |= (1<<6); 
  EXTI->IMR |= (1<<5);
  
  EXTI->FTSR |= (1<<11); // Fond descendant
  EXTI->FTSR |= (1<<12);
  EXTI->FTSR |= (1<<6);
  EXTI->FTSR |= (1<<5);
  
  SYSCFG->EXTICR[1] &= ~((1<<8)|(1<<10)|(1<<11)); // PC6
  SYSCFG->EXTICR[1] |= (1<<9);
  
  SYSCFG->EXTICR[1] &= ~((1<<4)|(1<<6)|(1<<7)); // PC5
  SYSCFG->EXTICR[1] |= (1<<5);
                   
  SYSCFG->EXTICR[2] &= ~((1<<12)|(1<<13)|(1<<14)|(1<<15));// PA11
  
  SYSCFG->EXTICR[3] &= ~((1<<0)|(1<<1)|(1<<2)|(1<<3));// PA12
}

void init_BP(void)
{
  /* Activation GPIOA */
  RCC->AHBENR |= (1<<0);
  /* Activation GPIOC */
  RCC->AHBENR |= (1<<2);
  
  /* Activation MODER BP1 & 2 */
  GPIOA->MODER &= ~((1<<22)|(1<<23)|(1<<24)|(1<<25));
  
  /* Activation MODER BP2 & 3 */
  GPIOC->MODER &= ~((1<<10)|(1<<11)|(1<<12)|(1<<13));
  
  /* Activation PUPDR BP1 & 2 */
  GPIOA->PUPDR &= ~((1<<22)|(1<<23)|(1<<24)|(1<<25));

  /* Activation PUPDR BP3 & 4 */
  GPIOC->PUPDR &= ~((1<<10)|(1<<11)|(1<<12)|(1<<13));   
}

void init_led(void)
{
  /* Activation GPIOB */
  RCC->AHBENR |= (1<<1);
  
  /* Activation MODER LED */
  GPIOB->MODER |= ((1<<2)|(1<<4)|(1<<20)|(1<<22)|(1<<24)|(1<<26)|(1<<28)|(1<<30));
  GPIOB->MODER &= ~((1<<3)|(1<<5)|(1<<21)|(1<<23)|(1<<25)|(1<<27)|(1<<29)|(1<<31));  
  GPIOB->OTYPER &= ~((1<<1)|(1<<2)|(1<<10)|(1<<11)|(1<<12)|(1<<13)|(1<<14)|(1<<15));
}

void wait(uint32_t tmp)
{
   uint32_t i = 0;
   for(i = 0; i<tmp; i++);
}
 
void affiche(uint32_t num)
{
   if(num == 1)
    {
      TIM3->PSC = 200;
      TIM3->CCER |= (1<<4);
      GPIOB->ODR |= ((1<<11)|(1<<12));
      wait(wait_tmp);
      GPIOB->ODR &= ~((1<<11)|(1<<12));
      TIM3->CCER &= ~(1<<4);
      wait(wait_tmp);      
    }
    
   if(num == 2)
    {
      TIM3->PSC = 400;
      TIM3->CCER |= (1<<4);
      GPIOB->ODR |= ((1<<11)|(1<<12)|(1<<10)|(1<<13));
      wait(wait_tmp);
      GPIOB->ODR &= ~((1<<11)|(1<<12)|(1<<10)|(1<<13));
      TIM3->CCER &= ~(1<<4);
      wait(wait_tmp);
    }
    
   if(num == 3)
    {
      TIM3->PSC = 600;
      TIM3->CCER |= (1<<4);
      GPIOB->ODR |= ((1<<11)|(1<<12)|(1<<10)|(1<<13)|(1<<2)|(1<<14));
      wait(wait_tmp);
      GPIOB->ODR &= ~((1<<11)|(1<<12)|(1<<10)|(1<<13)|(1<<2)|(1<<14));
      TIM3->CCER &= ~(1<<4);
      wait(wait_tmp);
    }
    
   if(num == 4)
    {
      TIM3->PSC = 800;
      TIM3->CCER |= (1<<4);
      GPIOB->ODR |= ((1<<1)|(1<<2)|(1<<10)|(1<<11)|(1<<12)|(1<<13)|(1<<14)|(1<<15));
      wait(wait_tmp);
      GPIOB->ODR &= ~((1<<1)|(1<<2)|(1<<10)|(1<<11)|(1<<12)|(1<<13)|(1<<14)|(1<<15));
      TIM3->CCER &= ~(1<<4);
      wait(wait_tmp);
    }

}
 
void affiche_sequence(uint32_t num)
{
  Afficheur();
  
  uint32_t i;
  for(i=0;i<100;i++)
  {
    randNumber = (uint8_t) rand();
  }
  randNumber = (randNumber % 4) + 1 ;
  sequence[compt] = randNumber;
  for(i=0;i<num;i++)
  {
    affiche(sequence[i]);
  }
}

void introduction(void)
{
  uint32_t tmp = 1000000;
  
  GPIOB->ODR &= ~((1<<1)|(1<<2)|(1<<10)|(1<<11)|(1<<12)|(1<<13)|(1<<14)|(1<<15));
  wait(tmp);
  GPIOB->ODR |= ((1<<1)|(1<<15));
  wait(tmp);
  GPIOB->ODR |= ((1<<2)|(1<<14));
  wait(tmp);
  GPIOB->ODR |= ((1<<10)|(1<<13));
  wait(tmp);
  GPIOB->ODR |= ((1<<11)|(1<<12));
  wait(tmp);
  GPIOB->ODR &= ~((1<<11)|(1<<12));
  wait(tmp);
  GPIOB->ODR &= ~((1<<10)|(1<<13));
  wait(tmp);
  GPIOB->ODR &= ~((1<<2)|(1<<14));
  wait(tmp);
  GPIOB->ODR &= ~((1<<1)|(1<<15));
  wait(tmp);
}

void suivant(void)
{
  uint32_t tmp = 100000;
  
  GPIOB->ODR &= ~((1<<1)|(1<<2)|(1<<10)|(1<<11)|(1<<12)|(1<<13)|(1<<14)|(1<<15));
  wait(tmp);
  GPIOB->ODR |= ((1<<1));
  wait(tmp);
  GPIOB->ODR &= ~((1<<1));
  GPIOB->ODR |= ((1<<2));
  wait(tmp);
  GPIOB->ODR &= ~((1<<2));
  GPIOB->ODR |= ((1<<10));
  wait(tmp);
  GPIOB->ODR &= ~((1<<10));
  GPIOB->ODR |= ((1<<11));
  wait(tmp);
  GPIOB->ODR &= ~((1<<11));
  GPIOB->ODR |= ((1<<12));
  wait(tmp);
  GPIOB->ODR &= ~((1<<12));
  GPIOB->ODR |= ((1<<13));
  wait(tmp);
  GPIOB->ODR &= ~((1<<13));
  GPIOB->ODR |= ((1<<14));
  wait(tmp);
  GPIOB->ODR &= ~((1<<14));
  GPIOB->ODR |= ((1<<15));
  wait(tmp);
  GPIOB->ODR &= ~((1<<15));
}

void erreur(void)
{
  uint32_t tmp = 100000;
  
  TIM3->PSC = 1599;
  TIM3->CCER |= (1<<0);
  GPIOB->ODR |= ((1<<1)|(1<<2)|(1<<10)|(1<<11)|(1<<12)|(1<<13)|(1<<14)|(1<<15));
  wait(tmp);
  GPIOB->ODR &= ~((1<<1)|(1<<2)|(1<<10)|(1<<11)|(1<<12)|(1<<13)|(1<<14)|(1<<15));
  wait(tmp);
  GPIOB->ODR |= ((1<<1)|(1<<2)|(1<<10)|(1<<11)|(1<<12)|(1<<13)|(1<<14)|(1<<15));
  wait(tmp);
  GPIOB->ODR &= ~((1<<1)|(1<<2)|(1<<10)|(1<<11)|(1<<12)|(1<<13)|(1<<14)|(1<<15));
  wait(tmp);
  GPIOB->ODR |= ((1<<1)|(1<<2)|(1<<10)|(1<<11)|(1<<12)|(1<<13)|(1<<14)|(1<<15));
  wait(tmp);
  TIM3->CCER &= ~(1<<0);
  suivant();
  affiche_sequence(1); 
}

void init_MOTEUR_EXT(void)
{
  RCC->AHBENR |= (1<<1);
  
  GPIOB->MODER |= (1<<9);
  GPIOB->MODER &= ~(1<<8);
  
  GPIOB->OTYPER &= ~(1<<4);
  
  GPIOB->AFR[0] |= (1<<17); 
}

void init_BIP(void)
{
  RCC->AHBENR |= (1<<2); // PC
  
  GPIOC->MODER |= (1<<15); //PC7
  GPIOC->MODER &= ~(1<<14); //PC7
  
  GPIOC->OTYPER &=~(1<<7); // pushpull
 
  GPIOC->AFR[0] |= (1<<29); // TIM3
}

void init_PWM(void)
{
  TIM3->CCMR1 &= ~((1<<0)|(1<<1));
  TIM3->CCMR1 &= ~(1<<4);
  TIM3->CCMR1 |= ((1<<5)|(1<<6));
  
  TIM3->CCR1 = 20;
  
  //TIM3->CCER |= (1<<0);
}

void init_BIP_TIM3(void)
{
  TIM3->CCMR1 &= ~((1<<8)|(1<<9)); 
  TIM3->CCMR1 &= ~(1<<12); 
  TIM3->CCMR1 |= ((1<<13)|(1<<14)); 
  
  TIM3->CCR2 = 38;
  
  //TIM3->CCER |= (1<<4); 
  
}

void init_TIM3(void)
{
  RCC->APB1ENR |= (1<<1);
  
  TIM3->PSC = 1599; /* PSC+1-1599+1-1600 soit F=10kHz donc T=100us */
  TIM3->ARR = 39; /* 40*100us = 4ms donc ARR = 40-1 = 39 */
  
  //TIM3->PSC = 15;
  //TIM3->ARR = 49;
  
  TIM3->CR1 |= (1<<7);
  TIM3->CR1 &= ~(1<<6);
  TIM3->CR1 &= ~(1<<5);
  TIM3->CR1 &= ~(1<<4);
  
  TIM3->CR1 |= (1<<0);
}

void init_SPI(void)
{
   RCC->AHBENR |= (1<<0); //On passe le bit un a 1 pour AHB pour les ports A
   RCC->APB2ENR |= (1<<12); //Horloge sur le MAX 
  
   //Paramétrages des trois pattes du MAX (PA5 PA8 et PA7)
  GPIOA->MODER &= ~(1<<10); //PA5 en Alternate fonction
  GPIOA->MODER |= (1<<11);
  GPIOA->MODER &= ~(1<<14); //PA7 en Alternate fonction
  GPIOA->MODER |= (1<<15);
  GPIOA->MODER |= (1<<16); //PA8 en sortie
  GPIOA->MODER &= ~(1<<17);

  GPIOA->OTYPER &= ~(1<<5);  //PA5
  GPIOA->OTYPER &= ~(1<<7);  //PA7
  GPIOA->OTYPER &= ~(1<<8);  //PA8
  
 //Multiplexage des pattes sur le AF5 du multiplexeur
  GPIOA->AFR[0] |= (1<<20); // Registre de selection de la fonction multiplexée
  GPIOA->AFR[0] |= (1<<22);
  GPIOA->AFR[0] |= (1<<28);
  GPIOA->AFR[0] |= (1<<30);
  
  SPI1->CR1 &= ~(1<<3); //BR
  SPI1->CR1 |= (1<<4);
  SPI1->CR1 &= ~(1<<5);
  
  SPI1->CR1 &= ~(1<<0); //CPOL  sur FMontant
  SPI1->CR1 &= ~(1<<1); //CPHA  sur Fmontant
   
  SPI1->CR1 |= (1<<2); //Bit MSTR
   
  SPI1->CR1 &= ~(1<<11); //DFF 
  
  SPI1->CR1 &= ~(1<<7); //LSBFIRST
  
  SPI1->CR1 |= (1<<14); // Mode only transmit
  SPI1->CR1 |= (1<<15); //
  
  SPI1->CR2 |= (1<<2); //SSOE
  
  SPI1->CR1 |= (1<<6); //Activation
     
}
  
void SPI_Write( uint8_t add,uint8_t data)
{
    Set_CS();
    Reset_CS();
    SendData_SPI(add);
    SendData_SPI(data);
    while ((SPI1->SR & SPI_SR_BSY) ==  SPI_SR_BSY);
    Set_CS();
}

void SendData_SPI (uint8_t add)
{
  while ((SPI1->SR & SPI_SR_TXE)!= SPI_SR_TXE);
  SPI1->DR = add;
}

void Set_CS (void)
{
  GPIOA->ODR |= (1<<8);
}

void Reset_CS (void)
{
  GPIOA->ODR &= ~(1<<8);
}

void Afficheur(void)
{
   uint32_t compt_unit;
   
   compt_unit = compt;
  
   if(compt == 0)
   {
    //Initialisation des digits à 0
    SPI_Write(0x03,0x00);
    SPI_Write(0x04,0x00);
   }
   if(compt >= 10)
   {
    compt_unit = compt - 10;
    SPI_Write(0x03,0x01);
   }
   if(compt >= 20)
   {
    compt_unit = compt - 20;
    SPI_Write(0x03,0x02);
   }
   if(compt >= 30)
   {
    compt_unit = compt - 30;
    SPI_Write(0x03,0x03);
   }
   if(compt >= 40)
   {
    compt_unit = compt - 40;
    SPI_Write(0x03,0x02);
   }


   SPI_Write(0x04,compt_unit);

}
