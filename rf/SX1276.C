#include "Sx1276.h"
#include "stdio.h"
#include "stm8l15x.h"
unsigned char   Frequency[3]={0x6c,0x80,0x00};

unsigned char   SpreadingFactor=7;    //7-12

unsigned char   CodingRate=2;        //1-4

unsigned char   Bw_Frequency=9;      //6-9

unsigned char   powerValue=7;

unsigned char   power_data[8]={0X80,0X80,0X80,0X83,0X86,0x89,0x8c,0x8f};

unsigned char   RF_EX0_STATUS;

unsigned char   CRC_Value;

unsigned char   SX1278_RLEN;

lpCtrlTypefunc_t lpTypefunc = {0,0,0,0,0};

void Delay1s(unsigned int ii)
{
   unsigned char j;
   while(ii--){
     for(j=0;j<100;j++);
   }
}

void register_rf_func(lpCtrlTypefunc_t *func)
{
   if(func->lpByteWritefunc != 0){
      lpTypefunc.lpByteWritefunc = func->lpByteWritefunc;
   }
   if(func->lpByteReadfunc != 0){
      lpTypefunc.lpByteReadfunc = func->lpByteReadfunc;
   }
   if(func->lpSwitchEnStatus != 0){
      lpTypefunc.lpSwitchEnStatus = func->lpSwitchEnStatus;
   }
   if(func->paSwitchCmdfunc != 0){
      lpTypefunc.paSwitchCmdfunc = func->paSwitchCmdfunc;
   }
   if(func->lpRecvDataTousr != 0){
      lpTypefunc.lpRecvDataTousr = func->lpRecvDataTousr;
   }
}

void FSK_SEND_PACKET(void)
{
   SX1276LoRaSetOpMode( Stdby_mode );
   SX1276WriteBuffer(REG_LR_DIOMAPPING1,0x01);
   SX1276WriteBuffer(REG_LR_DIOMAPPING2,0x20);
}
	
void SX1276WriteBuffer( unsigned char addr, unsigned char buffer)
{ 
   lpTypefunc.lpSwitchEnStatus(enOpen); //NSS = 0;
   lpTypefunc.lpByteWritefunc( addr | 0x80 );
   lpTypefunc.lpByteWritefunc( buffer);
   lpTypefunc.lpSwitchEnStatus(enClose); //NSS = 1;
}
	
unsigned char SX1276ReadBuffer(unsigned char addr)
{
  unsigned char Value;
  lpTypefunc.lpSwitchEnStatus(enOpen); //NSS = 0;
  lpTypefunc.lpByteWritefunc( addr & 0x7f  );
  Value = lpTypefunc.lpByteReadfunc();
  lpTypefunc.lpSwitchEnStatus(enClose);//NSS = 1;

  return Value; 
}
	
void SX1276LoRaSetOpMode( RFMode_SET opMode )
{
   unsigned char opModePrev;
   opModePrev=SX1276ReadBuffer(REG_LR_OPMODE);
   opModePrev &=0xf8;
   opModePrev |= (unsigned char)opMode;
   SX1276WriteBuffer( REG_LR_OPMODE, opModePrev);		
}

void SX1276LoRaFsk( Debugging_fsk_ook opMode )
{
   unsigned char opModePrev;
   opModePrev=SX1276ReadBuffer(REG_LR_OPMODE);
   opModePrev &=0x7F;
   opModePrev |= (unsigned char)opMode;
   SX1276WriteBuffer( REG_LR_OPMODE, opModePrev);		
}
	
void SX1276LoRaSetRFFrequency(void)
{
   SX1276WriteBuffer( REG_LR_FRFMSB, Frequency[0]);
   SX1276WriteBuffer( REG_LR_FRFMID, Frequency[1]);
   SX1276WriteBuffer( REG_LR_FRFLSB, Frequency[2]);
}
		
void SX1276LoRaSetRFPower(unsigned char power )
{
   SX1276WriteBuffer( REG_LR_PADAC, 0x87);
   SX1276WriteBuffer( REG_LR_PACONFIG,  power_data[power] );
}
	
void SX1276LoRaSetSpreadingFactor(unsigned char factor )
{
   unsigned char RECVER_DAT;
   SX1276LoRaSetNbTrigPeaks( 3 );
   RECVER_DAT=SX1276ReadBuffer( REG_LR_MODEMCONFIG2);	  
   RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG2_SF_MASK ) | ( factor << 4 );
   SX1276WriteBuffer( REG_LR_MODEMCONFIG2, RECVER_DAT );	 
}
	
void SX1276LoRaSetNbTrigPeaks(unsigned char value )
{
   unsigned char RECVER_DAT;
   RECVER_DAT = SX1276ReadBuffer( 0x31);
   RECVER_DAT = ( RECVER_DAT & 0xF8 ) | value;
   SX1276WriteBuffer( 0x31, RECVER_DAT );
}
	
void SX1276LoRaSetErrorCoding(unsigned char value )
{	
   unsigned char RECVER_DAT;
   RECVER_DAT=SX1276ReadBuffer( REG_LR_MODEMCONFIG1);
   RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG1_CODINGRATE_MASK ) | ( value << 1 );
   SX1276WriteBuffer( REG_LR_MODEMCONFIG1, RECVER_DAT);
   // LoRaSettings.ErrorCoding = value;
}
	
void SX1276LoRaSetPacketCrcOn(bool enable )
{	
   unsigned char RECVER_DAT;
   RECVER_DAT= SX1276ReadBuffer( REG_LR_MODEMCONFIG2);
   RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK ) | ( enable << 2 );
   SX1276WriteBuffer( REG_LR_MODEMCONFIG2, RECVER_DAT );
}
	
void SX1276LoRaSetSignalBandwidth(unsigned char bw )
{
   unsigned char RECVER_DAT;
   RECVER_DAT=SX1276ReadBuffer( REG_LR_MODEMCONFIG1);
   RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG1_BW_MASK ) | ( bw << 4 );
   SX1276WriteBuffer( REG_LR_MODEMCONFIG1, RECVER_DAT );
   // LoRaSettings.SignalBw = bw;
}
	
void SX1276LoRaSetImplicitHeaderOn(bool enable )
{
   unsigned char RECVER_DAT;
   RECVER_DAT=SX1276ReadBuffer( REG_LR_MODEMCONFIG1 );
   RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK ) | ( enable );
   SX1276WriteBuffer( REG_LR_MODEMCONFIG1, RECVER_DAT );
}
	
void SX1276LoRaSetSymbTimeout(unsigned int value )
{
   unsigned char RECVER_DAT[2];
   RECVER_DAT[0]=SX1276ReadBuffer( REG_LR_MODEMCONFIG2 );
   RECVER_DAT[1]=SX1276ReadBuffer( REG_LR_SYMBTIMEOUTLSB );
   RECVER_DAT[0] = ( RECVER_DAT[0] & RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK ) | ( ( value >> 8 ) & ~RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK );
   RECVER_DAT[1] = value & 0xFF;
   SX1276WriteBuffer( REG_LR_MODEMCONFIG2, RECVER_DAT[0]);
   SX1276WriteBuffer( REG_LR_SYMBTIMEOUTLSB, RECVER_DAT[1]);
}
	
void SX1276LoRaSetPayloadLength(unsigned char value )
{
   SX1276WriteBuffer( REG_LR_PAYLOADLENGTH, value );
} 
	
void SX1276LoRaSetPreamLength(unsigned int value )
{
   unsigned char a[2];
   a[0]=(value&0xff00)>>8;
   a[1]=value&0xff;
   SX1276WriteBuffer( REG_LR_PREAMBLEMSB, a[0] );
   SX1276WriteBuffer( REG_LR_PREAMBLELSB, a[1] );
}
	
void SX1276LoRaSetMobileNode(bool enable )
{	 
   unsigned char RECVER_DAT;
   RECVER_DAT=SX1276ReadBuffer( REG_LR_MODEMCONFIG3 );
   RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG3_MOBILE_NODE_MASK ) | ( enable << 3 );
   SX1276WriteBuffer( REG_LR_MODEMCONFIG3, RECVER_DAT );
}

void SX1276LORA_INT(void)
{
   SX1276LoRaSetOpMode(Sleep_mode);  //设置睡眠模式
   SX1276LoRaFsk(LORA_mode);	     // 设置扩频模式
   SX1276LoRaSetOpMode(Stdby_mode);  // 设置为普通模式
   SX1276WriteBuffer( REG_LR_DIOMAPPING1,GPIO_VARE_1);
   SX1276WriteBuffer( REG_LR_DIOMAPPING2,GPIO_VARE_2);
   SX1276LoRaSetRFFrequency();
   SX1276LoRaSetRFPower(powerValue);
   SX1276LoRaSetSpreadingFactor(SpreadingFactor);	 // 扩频因子设置
   SX1276LoRaSetErrorCoding(CodingRate);		 //有效数据比
   SX1276LoRaSetPacketCrcOn(TRUE);			 //CRC 校验打开
   SX1276LoRaSetSignalBandwidth( Bw_Frequency );	 //设置扩频带宽
   SX1276LoRaSetImplicitHeaderOn(FALSE);		 //同步头是显性模式
   SX1276LoRaSetPayloadLength( 0xff);
   SX1276LoRaSetSymbTimeout( 0x3FF );
   SX1276LoRaSetMobileNode(TRUE); 			 // 低数据的优化
   RF_RECEIVE();
}
	
void FUN_RF_SENDPACKET(unsigned char *RF_TRAN_P,unsigned char LEN)
{
   unsigned char ASM_i;
   lpTypefunc.paSwitchCmdfunc(txOpen);
   SX1276LoRaSetOpMode( Stdby_mode );
   SX1276WriteBuffer( REG_LR_HOPPERIOD, 0 );	//不做频率跳变
   SX1276WriteBuffer(REG_LR_IRQFLAGSMASK,IRQN_TXD_Value);	//打开发送中断
   SX1276WriteBuffer( REG_LR_PAYLOADLENGTH, LEN);	 //最大数据包
   SX1276WriteBuffer( REG_LR_FIFOTXBASEADDR, 0);
   SX1276WriteBuffer( REG_LR_FIFOADDRPTR, 0 );
   lpTypefunc.lpSwitchEnStatus(enOpen);
   lpTypefunc.lpByteWritefunc( 0x80 );
   for( ASM_i = 0; ASM_i < LEN; ASM_i++ ){
     lpTypefunc.lpByteWritefunc( *RF_TRAN_P );RF_TRAN_P++;
   }
   lpTypefunc.lpSwitchEnStatus(enClose);
   SX1276WriteBuffer(REG_LR_DIOMAPPING1,0x40);
   SX1276WriteBuffer(REG_LR_DIOMAPPING2,0x00);
   SX1276LoRaSetOpMode( Transmitter_mode );
   Delay1s(200);
}

void RF_RECEIVE (void)
{
   SX1276LoRaSetOpMode(Stdby_mode );
   SX1276WriteBuffer(REG_LR_IRQFLAGSMASK,IRQN_RXD_Value);  //打开发送中断
   SX1276WriteBuffer(REG_LR_HOPPERIOD,	PACKET_MIAX_Value );
   SX1276WriteBuffer( REG_LR_DIOMAPPING1, 0X00 );
   SX1276WriteBuffer( REG_LR_DIOMAPPING2, 0X00 );	
   SX1276LoRaSetOpMode( Receiver_mode );
   lpTypefunc.paSwitchCmdfunc(rxOpen);
   Delay1s(200);
}
	
void RF_CAD_RECEIVE (void)
{
   SX1276LoRaSetOpMode( Stdby_mode );
   SX1276WriteBuffer(REG_LR_IRQFLAGSMASK,  IRQN_CAD_Value);	//打开发送中断
   SX1276WriteBuffer( REG_LR_DIOMAPPING1, 0X80 );
   SX1276WriteBuffer( REG_LR_DIOMAPPING2, 0X00 );	
   SX1276LoRaSetOpMode( CAD_mode );
   lpTypefunc.paSwitchCmdfunc(rxOpen);
}
	
void RF_SEELP(void)
{
   SX1276LoRaSetOpMode( Stdby_mode );
   SX1276WriteBuffer(REG_LR_IRQFLAGSMASK,  IRQN_SEELP_Value);  //打开发送中断
   SX1276WriteBuffer( REG_LR_DIOMAPPING1, 0X00 );
   SX1276WriteBuffer( REG_LR_DIOMAPPING2, 0X00 );	
   SX1276LoRaSetOpMode( Sleep_mode );
}

//**************下面是中断里面处理的代码*******************************
unsigned char recv[256];

void SX1278_Interupt(void)
{
  RF_EX0_STATUS=SX1276ReadBuffer( REG_LR_IRQFLAGS ); 
  if((RF_EX0_STATUS&0x40)==0x40){                            //RXDone 
    CRC_Value=SX1276ReadBuffer( REG_LR_MODEMCONFIG2 );
    if((CRC_Value&0x04 )==0x04){                             //CRC on
      SX1276WriteBuffer (REG_LR_FIFOADDRPTR,0x00);
      SX1278_RLEN = SX1276ReadBuffer(REG_LR_NBRXBYTES);
      lpTypefunc.lpSwitchEnStatus(enOpen);
      lpTypefunc.lpByteWritefunc( 0x00 );
      for(unsigned char RF_REC_RLEN_i=0;RF_REC_RLEN_i<SX1278_RLEN;RF_REC_RLEN_i++){
        recv[RF_REC_RLEN_i]=lpTypefunc.lpByteReadfunc();
			USART_SendData8(USART1,recv[RF_REC_RLEN_i]);
            while(((USART1->SR) & 0x80) == 0x00);
      }
      lpTypefunc.lpSwitchEnStatus(enClose);

    } 
    SX1276LoRaSetOpMode( Stdby_mode );
    SX1276WriteBuffer(REG_LR_IRQFLAGSMASK, IRQN_RXD_Value);  //打开发送中断
    SX1276WriteBuffer(REG_LR_HOPPERIOD,    PACKET_MIAX_Value);
    SX1276WriteBuffer( REG_LR_DIOMAPPING1, 0X00 );
    SX1276WriteBuffer( REG_LR_DIOMAPPING2, 0x00 );	
    SX1276LoRaSetOpMode( Receiver_mode );
    lpTypefunc.paSwitchCmdfunc(rxOpen);
  }else{
    SX1276LoRaSetOpMode( Stdby_mode );
    SX1276WriteBuffer(REG_LR_IRQFLAGSMASK,IRQN_RXD_Value);  //打开发送中断
    SX1276WriteBuffer(REG_LR_HOPPERIOD,   PACKET_MIAX_Value );
    SX1276WriteBuffer( REG_LR_DIOMAPPING1, 0X02 );
    SX1276WriteBuffer( REG_LR_DIOMAPPING2, 0x00 );	
    SX1276LoRaSetOpMode( Receiver_mode );
    lpTypefunc.paSwitchCmdfunc(rxOpen);
  }
  SX1276WriteBuffer( REG_LR_IRQFLAGS, 0xff  );
}

