#include "SPI_Flash_HAL.H"
static unsigned char HAL_Get_IO(GPIO_Port_TypeDef by_Port, unsigned int w_Pin)
{
    int data;
    if (w_Pin < 8)
    {
        data= (GPIO->P[by_Port].MODEL & (0xF << (w_Pin * 4)))>>(w_Pin * 4);
    }
    else
    {
        data= (GPIO->P[by_Port].MODEH & (0xF << ((w_Pin - 8) * 4)))>>((w_Pin - 8) * 4);
    }
    
    if(data<4)
        return( GPIO_PinInGet(by_Port,w_Pin) );
    else
        return( GPIO_PinOutGet(by_Port,w_Pin) );
}
void HAL_SPI_Flash_Initial(void)
{
  
  GPIO_PinModeSet(CS_PORT,CS_PIN,gpioModePushPull,0);
  GPIO_PinOutSet(CS_PORT,CS_PIN);
  
  GPIO_PinModeSet(CLK_PORT,CLK_PIN,gpioModePushPull,0);
  GPIO_PinOutSet(CLK_PORT,CLK_PIN);
  
  GPIO_PinModeSet(DO_PORT,DO_PIN,gpioModeInput,1);
  
  GPIO_PinModeSet(DI_PORT,DI_PIN,gpioModePushPull,0);
  GPIO_PinOutSet(DI_PORT,DI_PIN);
  
}

void HAL_SPI_Flash_CS(bool isHigh)
{
  if(isHigh)
  {
    GPIO_PinOutSet(CS_PORT, CS_PIN); 
  }
  else
  {
    GPIO_PinOutClear(CS_PORT, CS_PIN); 
  }
}


void HAL_SPI_Flash_CLK(bool isHigh)
{
  if(isHigh)
  {
    GPIO_PinOutSet(CLK_PORT, CLK_PIN);
  }
  else
  {
    GPIO_PinOutClear(CLK_PORT, CLK_PIN);
  }
}
void HAL_SPI_Flash_DI(bool isHigh)
{
  if(isHigh)
  {
    GPIO_PinOutSet(DI_PORT, DI_PIN);
  }
  else
  {
    GPIO_PinOutClear(DI_PORT, DI_PIN);
  }
}

bool HAL_SPI_Flash_DO(void)
{
  return (HAL_Get_IO(DO_PORT, DO_PIN));
}