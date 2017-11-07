#include "key.h"

void GPIO_Config(void)
{
    GPIO_Init(GPIOD, GPIO_Pin_6, GPIO_Mode_In_PU_No_IT); //key
    GPIO_Init(GPIOA, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow); //yellow
    GPIO_Init(GPIOA, GPIO_Pin_5, GPIO_Mode_Out_PP_High_Slow); //GREEN

    GPIO_Init(GPIOB, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow); //辅助   0:1 灭
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

    GPIO_Init(GPIOB, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow); //辅助   0:1 灭
    GPIO_Init(GPIOB, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Slow);
}

/*读取键值*/
unsigned char KeyRead( void )
{
    unsigned char ReadData;
    ReadData = (KEYPORT->IDR & KEYPIN) & KEY_NORMAL;
    return ReadData;
}
/*根据不停扫描键值返回按键状态*/
unsigned char keyScan(void)
{
    static unsigned char key_state = waitForPass;     //按键的状态
    static unsigned char keyTimeCnt = 0;
    unsigned char key_press;                 //按键是否被按下
    unsigned char key_return = waitForPass; //按键返回值
    key_press = KeyRead();                     // 读按键

    switch (key_state) {
        case waitForPass:                 // 按键初始态
            if (!key_press) {
                key_state = KeyPassLong;   // 键被按下，状态转换到键确认态
            }
            break;
//        case keyPassOne:                 // 单次触发确认
//             if (!key_press){
//                  key_state = KeyPassLong;   // 键被按下，状态转换到长按判断认态
//             }else key_state = waitForPass;
//             break;
        case KeyPassLong:                 //消抖及长按确认
            if (!key_press) {
                keyTimeCnt++;
                if( keyTimeCnt > KEY_LONG_PASS_TIME ) {  //如果长按时间到，返回值为长按状态
                    keyTimeCnt = 0;
                    key_state = keyPassReturn;         //并切换到释放状态
                    key_return = KeyPassLong;          //返回值为长按状态
                }
            } else {                                   //如果长按时间不足，视为单次触发
                keyTimeCnt = 0;
                key_state = waitForPass;                //单次触发直接切换为初始状态
                key_return = keyPassOne;                //返回值为单次触发
            }
            break;
        case keyPassReturn:               //释放状态，需等到按键释放，键值为normal状态
            if(KEY_NORMAL == key_press)
                key_state =  waitForPass;
            break;
    }
    return key_return;                            //返回按键值
}

