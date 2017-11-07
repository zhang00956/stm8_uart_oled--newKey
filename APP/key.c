#include "key.h"

void GPIO_Config(void)
{
    GPIO_Init(GPIOD, GPIO_Pin_6, GPIO_Mode_In_PU_No_IT); //key
    GPIO_Init(GPIOA, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow); //yellow
    GPIO_Init(GPIOA, GPIO_Pin_5, GPIO_Mode_Out_PP_High_Slow); //GREEN

    GPIO_Init(GPIOB, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow); //����   0:1 ��
    GPIO_Init(GPIOB, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Slow);
    
    GPIO_Init(GPIOA, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Slow); //beep
}
void BeepInit(void)
{
    GPIO_Init(GPIOA, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Slow); //beep
}
void LEDInit(void)
{
    GPIO_Init(GPIOA, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow); //yellow
    GPIO_Init(GPIOA, GPIO_Pin_5, GPIO_Mode_Out_PP_High_Slow); //GREEN

    GPIO_Init(GPIOB, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow); //����   0:1 ��
    GPIO_Init(GPIOB, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Slow);
}

/*��ȡ��ֵ*/
unsigned char KeyRead( void )
{
    unsigned char ReadData;
    ReadData = (KEYPORT->IDR & KEYPIN) & KEY_NORMAL;
    return ReadData;
}
/*���ݲ�ͣɨ���ֵ���ذ���״̬*/
unsigned char keyScan(void)
{
    static unsigned char key_state = waitForPass;     //������״̬
    static unsigned char keyTimeCnt = 0;
    unsigned char key_press;                 //�����Ƿ񱻰���
    unsigned char key_return = waitForPass; //��������ֵ
    key_press = KeyRead();                     // ������

    switch (key_state) {
        case waitForPass:                 // ������ʼ̬
            if (!key_press) {
                key_state = KeyPassLong;   // �������£�״̬ת������ȷ��̬
            }
            break;
//        case keyPassOne:                 // ���δ���ȷ��
//             if (!key_press){
//                  key_state = KeyPassLong;   // �������£�״̬ת���������ж���̬
//             }else key_state = waitForPass;
//             break;
        case KeyPassLong:                 //����������ȷ��
            if (!key_press) {
                keyTimeCnt++;
                if( keyTimeCnt > KEY_LONG_PASS_TIME ) {  //�������ʱ�䵽������ֵΪ����״̬
                    keyTimeCnt = 0;
                    key_state = keyPassReturn;         //���л����ͷ�״̬
                    key_return = KeyPassLong;          //����ֵΪ����״̬
                }
            } else {                                   //�������ʱ�䲻�㣬��Ϊ���δ���
                keyTimeCnt = 0;
                key_state = waitForPass;                //���δ���ֱ���л�Ϊ��ʼ״̬
                key_return = keyPassOne;                //����ֵΪ���δ���
            }
            break;
        case keyPassReturn:               //�ͷ�״̬����ȵ������ͷţ���ֵΪnormal״̬
            if(KEY_NORMAL == key_press)
                key_state =  waitForPass;
            break;
    }
    return key_return;                            //���ذ���ֵ
}
