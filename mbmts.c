//Define memory addresses for virtual input, fan output, and temperature output
#define virtualinput_adress 0x01FF
#define fanoutput_adress 0x01F0
#define tempoutput_adress 0x01D0

/* I defined some flags for my program:
   0 -> New virtualinput
   1 -> BuzzerCALL
   2 -> TempLCD Cntrl
   3 -> FanspeedLCD Cntrl
   4 -> TempcodeLCD Cntrl
   5 -> BuzzerCntrl
*/

unsigned char progflags;
unsigned char VTCursor;     //Cursor for virtual terminal
unsigned char tmp;  //it holds the temperature
unsigned char MotorDutyCycle;       // it holds OCR0 value for motor
  unsigned char MotorFanSpeed;        //Percentage(%) of fan speed
unsigned char BuzzersecCounter;  //Buzzer  counter counts for 2 second

// Setting LCD pins from LCD library
sbit LCD_RS at PORTB2_BIT ;
sbit LCD_EN at PORTB5_BIT ;
sbit LCD_RS_Direction at DDB2_bit;
sbit LCD_EN_Direction at DDB5_bit;

sbit LCD_D4 at PORTC4_BIT ;
sbit LCD_D5 at PORTC5_BIT ;
sbit LCD_D6 at PORTC6_BIT ;
sbit LCD_D7 at PORTC7_BIT ;
sbit LCD_D4_Direction at DDC4_bit;
sbit LCD_D5_Direction at DDC5_bit;
sbit LCD_D6_Direction at DDC6_bit;
sbit LCD_D7_Direction at DDC7_bit;

//Toggle global interrupts on or off
void ToggleInterrupts(){
        SREG_I_bit = !SREG_I_bit;
}

//This function gives desired bit of Program Flags
unsigned char getBit(unsigned char bit_numb){
        unsigned char flag = progflags & (1<< bit_numb);
        if(flag != 0)
            return 0x01;
        else
            return 0x00;
}

//This function sets desired bit of Program Flags
void setBit(unsigned char bit_numb){
        progflags |= (1<< bit_numb);
}

//This function clears desired bit of Program Flags
void clearBit(unsigned char bit_numb){
        progflags &= ~(1<< bit_numb);
}

//This function gives lenght of a Integer
unsigned char SizeOfInteger(unsigned char input){
        unsigned char size = 0;
        do {
                size++;
                input /= 10;
        } while (input);

        return size;
}

//This function converts Integer Fan % and Temp to String
void SyncStrings(){
        unsigned char *FANOutput = (unsigned char *) fanoutput_adress;
        unsigned char *TEMPOutput = (unsigned char *) tempoutput_adress;
        unsigned char divfactor,size,i;
 
 // Calculate size of MotorFanSpeed and store it in FANOutput
        size = SizeOfInteger(MotorFanSpeed);
        for(i = 0, divfactor = 1; size > i; i++, divfactor *= 10){
                FANOutput[size-i-1] = (MotorFanSpeed/divfactor)%10 + '0';
        }
        FANOutput[size] = '\0';
        
        // Calculate size of tmp and store it in TEMPOutput
        size = SizeOfInteger(tmp);
        for(i = 0, divfactor = 1; size > i; i++, divfactor *= 10){
                TEMPOutput[size-i-1] = (tmp /divfactor)%10 + '0';
        }
        TEMPOutput[size] = '\0';
}

//This function initializes USART(Universal Synchronous and Asynchronous Receiver Transmitter)
void InitializeVT(){
        VTCursor = 0;
          UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
        UBRR0L = 51;
        UCSR0B = (1<<TXEN0|1<<RXEN0|1<<RXCIE0);
}

//This function sends char by pooling
void SendChar(unsigned char input){
        while(UCSR0A.UDRE0 != 1);
        UDR0 = input;
}

//This function recives chars to VTInput UC array
//After reciving enter it will set progflags 0.
void ReciveCharInterrupt() iv IVT_ADDR_USART0__RX{
        unsigned char *VTInput = (unsigned char *) virtualinput_adress;

        VTInput[VTCursor] = UDR0;
        if(VTInput[VTCursor] == 13){
                VTInput[VTCursor] = '\0';
                setBit(0);
        } else{
                VTCursor++;
        }
}
// This function sends a line break character (Carriage Return) via USART
void SendLineBreak(){
        SendChar(13);
}

//Sends array of string by using SendChar
void SendText(unsigned char input[]){
        int i;
        for(i = 0; input[i] != '\0'; i++){
                SendChar(input[i]);
        }
}

//Interprets the given VTInput from Interrupt
//Only works if progflags 0 seted.
//After interpretting it will clear progflags 0
void InterpretVTInput(){
        if(getBit(0)){
                unsigned char *VTInput = (unsigned char *) virtualinput_adress;
                unsigned char *FANOutput = (unsigned char *) fanoutput_adress;
                unsigned char *TEMPOutput = (unsigned char *) tempoutput_adress;

                if(strcmp(VTInput,"s") == 0 || strcmp(VTInput,"S") == 0){
                        SendText(FANOutput);
                } else if(strcmp(VTInput,"t") == 0 || strcmp(VTInput,"T") == 0){
                        SendText(TEMPOutput);
                } else{
                        SendText("Warning Wrong Input!");
                }
                SendLineBreak();
                SendText("Enter S for % fan speed and T for temperature:");
                SendLineBreak();
                clearBit(0);
                VTCursor = 0;
        }
}

//Initializes Buzzer, and make sure it buzz 2 sec.
void InitializeBuzzer(){
        TCCR1B = (1 << CS11);
        TIMSK |= (1 << TOIE1);
        BuzzersecCounter = 0;

        DDRA |= (1<<1);
        OCR2 = 0x12;
        TIMSK |= (1 << OCIE2);
        TCCR2 = (1 << WGM21) | (1 << CS22);
}

//Stops Buzzer
void StopBuzzer(){
        TCCR2 = 0x00;
        TCCR1B = 0x00;
        TIMSK &= ~((1 << TOIE1) | (1 << OCIE2));
        BuzzersecCounter = 0;
        PORTA &= ~(1<<1);
        DDRA &= ~(1<<1);
}

//Creates frequency for buzzer
void BuzzerBeep() iv IVT_ADDR_TIMER2_COMP{
        if((PORTA & 0x02) == 0)
                PORTA |= (1<<1);
        else
                PORTA &= ~(1<<1);
}

//Makes sure buzzer works only for 2 sec.
void BuzzerTimer() iv IVT_ADDR_TIMER1_OVF{
        BuzzersecCounter++;
        if(BuzzersecCounter == 38)
                StopBuzzer();
}

//Checks if buzzer should work or not.
//Only works progflags bit 5 seted.
//If tmp > 30 buzzer works for once.
//Only works again if Temperature < 30 and again Temp > 30.
void CheckBuzzer(){
        if(getBit(5)){
                if(!getBit(1) && tmp >= 30){
                        InitializeBuzzer();
                        setBit(1);
                } else if(getBit(1) && tmp < 30){
                        clearBit(1);
                }
        }
}

//Initializes LCD Library Settings
void InitializeLCD(){
        Lcd_Init();
        Lcd_Cmd(_LCD_CURSOR_OFF);
}

//Sync LCD Data by consedering progflags
//Bit 2 and bit 3. Always Refreshes Data.
void SyncLCD(){
        unsigned char *FANOutput = (unsigned char *) fanoutput_adress;
        unsigned char *TEMPOutput = (unsigned char *) tempoutput_adress;
        if(getBit(2)){
                Lcd_Out(1,1,TEMPOutput);
        } else{
                Lcd_Out(1,1,"");
        }
        if(getBit(3)){
                Lcd_Out(2,1,FANOutput);
        } else{
                Lcd_Out(2,1,"");
        }
}

//Initialize ADConverter.
void InitializeThermometer(){
        DDRF = 0x00;
        ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1);
        ADMUX =  (1 << REFS0);
        ADCSRA |= (1<<ADSC);
}

//Takes Converted data by Interrupt,
//Scale it from 0-1023 to 0-100.
void SyncThermometer() iv IVT_ADDR_ADC{
        unsigned char low,high;
        low = ADCL;
        high = ADCH;
        tmp = floor((low | (high << 8))*(100.00/1023.00));
        ADCSRA |= (1 << ADSC);
}

//Initializes Leds.
void InitializeLeds(){
        DDRD = 0xFF;
}

//Sync Leds. Uses Temperature. Lights on
//Leds per 12.5 C.
void SyncLeds(){
        if(getBit(4)){
                unsigned int i,step = floor(tmp /12.5);
                PORTD = 0x00;
                for(i = 0; step > i; i++)
                        PORTD |= (1<<i);
        } else{
                PORTD = 0x00;
        }
}

//Initializes PWM for Motor.
void InitializeMotor(){
        DDRB |= (1<<4);
        TCCR0 = (1<<WGM00) | (1<<COM01) | (1<<COM00) | (1<<CS02);
        OCR0 = 255;
}

//Sync Motor values with Temperature.
//Scale Temperature 30-100 to 0-255
//Scale MotorDutyCycle 0-255 to 0-100
//OCR0 will be 255-MotorDutyCycle
void SyncMotor(){
        if(tmp >= 30){
                MotorDutyCycle = floor((tmp -30)*(255.00/70.00));
                MotorFanSpeed = floor(MotorDutyCycle*(100.00/255.00));
        } else{
                MotorDutyCycle = 0;
                MotorFanSpeed = 0;
        }
        OCR0 = 255-MotorDutyCycle;
}

//Initialize Configs (4 DIPSWITCH)
void InitializeConfigs(){
        DDRA &= ~((1<<4) | (1<<5) | (1<<6) | (1<<7));
}

//Takes A4-7 to progflags 2-5
void SyncConfigs(){
        if((PINA & (1<<4))){
                setBit(2);
        } else{
                clearBit(2);
        }
        if((PINA & (1<<5))){
                setBit(3);
        } else{
                clearBit(3);
        }
        if((PINA & (1<<6))){
                setBit(4);
        } else{
                clearBit(4);
        }
        if((PINA & (1<<7)) != 0){
                setBit(5);
        } else{
                clearBit(5);
        }
}

void mainProgram(){
        //Open Global Interrupt
        ToggleInterrupts();
        //Initialize Modules
        InitializeThermometer();
        InitializeVT();
        InitializeLCD();
        InitializeLeds();
        InitializeMotor();
        InitializeConfigs();
        //Send First message with USART
        SendText("Enter S for % fan speed and T for temperature:");
        SendLineBreak();
        //Set Temperature as 0
        tmp = 0;
        while(1){
                //Synchronize whole program in every 10 us.
                SyncConfigs();
                SyncStrings();
                InterpretVTInput();
                CheckBuzzer();
                SyncLCD();
                SyncLeds();
                SyncMotor();
                Delay_us(10);
        }
}

void main() {
        //Initialize progflags as 0.
        progflags = 0;
        //Call maing program.
        mainProgram();
}
