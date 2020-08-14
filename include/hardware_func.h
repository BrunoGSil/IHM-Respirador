//Funcoes relacionadas ao timer
hw_timer_t * timerInt = NULL;
hw_timer_t * timerFPS = NULL;

unsigned int timercount = 0;
void int1s() {
	timercount++;
}

//Funcoes relacionadas aos botoes
byte buttonFlag = 0;
uint32_t port32val = 0;
TaskHandle_t ButtonTask;

//Detecta interrupcoes do botao
unsigned long lastButInt = 0;
void ButtonInterrupt() {
	if(millis()-lastButInt>500) {
		lastButInt=millis();
		vTaskResume(ButtonTask);
	}
}

//Tarefa para botoes
void ButtonTaskFunction(void* parameters) {
	while(1) {
		port32val=(uint32_t)(*portInputRegister(digitalPinToPort(button0pin)));
		buttonFlag = ((port32val&0x30000)>>16) | ((port32val&0x1000)>>10);
		vTaskSuspend(NULL);
	}
}

//Funcoes relacionadas ao encoder
unsigned short decoder = 0, decoderPast = 0;
TaskHandle_t EncoderTask;

//Detecta interrupcoes do encoder
unsigned long lastEncInt = 0;
void EncoderInterrupt() {
	if(millis()-lastEncInt>1) {
		lastEncInt=millis();
		vTaskResume(EncoderTask);
	}
}

uint8_t portVal = 0, mp = 0, i = 0, pastI = 0;
//Tarefa para decodificar o encoder incremental
void EncoderTaskFunction(void* parameters) {
	while(1) {
		portVal=(uint8_t)(*portInputRegister(digitalPinToPort(encApin)));
		i = portVal&0x30;
		if(i!=pastI)
		{
			pastI=i;
			switch(i){
				case 0x10:
					if(mp==0)
					mp=0b01;
					break;
				case 0x20:
					if(mp==0)
					mp=0b10;
					break;
				case 0x00:
					if(mp==1)
					decoder++;
				else if(mp==2)
					decoder--;
					mp=0;
				break;
			}
		}
		vTaskSuspend(NULL);
	}
}
