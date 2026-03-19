#include "driver/gpio.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"  

//autor: daniel luccas/turma A

//definição de pinagem
#define LED_VERDE_CARRO 27
#define LED_AMARELO_CARRO 13
#define LED_VERMELHO_CARRO 32

#define LED_VERDE_PEDESTRE 25
#define LED_VERMELHO_PEDESTRE 26

#define BOTAO 4

//codigo bool utilizado para verdadeiro ou falso
bool pedidoPedestre = false;

void button_task(void *arg)
{
    while (1)
    {
        if (gpio_get_level(BOTAO) == 0)
        {
            printf("Botão pressionado\n");
             pedidoPedestre = true;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}


// define o nome dos estados
typedef enum {
    VERDE_CARRO,
    AMARELO_CARRO,
    VERMELHO_CARRO,
    PEDESTRE
} estado_t;
estado_t estadoAtual = VERDE_CARRO;



void app_main() {
//    sinal carros 
    gpio_set_direction(LED_VERDE_CARRO, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_AMARELO_CARRO, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_VERMELHO_CARRO, GPIO_MODE_OUTPUT);

//  sinal pedestre 
    gpio_set_direction(25, GPIO_MODE_OUTPUT);
    gpio_set_direction(26, GPIO_MODE_OUTPUT);

//  botão 
    gpio_set_direction(BOTAO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BOTAO,GPIO_PULLUP_ONLY);
      xTaskCreate(button_task, "button_task", 2048, NULL, 5, NULL);
    while (true) {

        /*if (gpio_get_level(BOTAO) == 0) {
           pedidoPedestre = true;
        }
        
        if (gpio_get_level(BOTAO) == 1 ) {
           pedidoPedestre = false;
        }*/

//switch define qual case 
        switch (estadoAtual) {

            case VERDE_CARRO:

                gpio_set_level(LED_VERDE_CARRO, 1);
                gpio_set_level(LED_AMARELO_CARRO, 0);
                gpio_set_level(LED_VERMELHO_CARRO, 0);

                gpio_set_level(LED_VERDE_PEDESTRE, 0);
                gpio_set_level(LED_VERMELHO_PEDESTRE, 1);

                 if (pedidoPedestre == false )
                 {
               vTaskDelay(3000 / portTICK_PERIOD_MS);
                estadoAtual = AMARELO_CARRO;  
                printf("tempo NORMAL\n");
                } 

               else if (pedidoPedestre == true) {
                  pedidoPedestre=false;
                printf("tempo encurtado\n");
                    vTaskDelay(1000 / portTICK_PERIOD_MS);
                     estadoAtual = AMARELO_CARRO;
                }

               vTaskDelay(1000 / portTICK_PERIOD_MS);

            break;

            case AMARELO_CARRO:

                gpio_set_level(LED_VERDE_CARRO, 0);
                gpio_set_level(LED_AMARELO_CARRO, 1);

                vTaskDelay(2000 / portTICK_PERIOD_MS);

                gpio_set_level(LED_AMARELO_CARRO, 0);

                estadoAtual = VERMELHO_CARRO;

            break;

            case VERMELHO_CARRO:

                gpio_set_level(LED_VERMELHO_CARRO, 1);

                estadoAtual = PEDESTRE;

            break;

            case PEDESTRE:

                gpio_set_level(LED_VERDE_PEDESTRE, 1);
                gpio_set_level(LED_VERMELHO_PEDESTRE, 0);
                vTaskDelay(5000 / portTICK_PERIOD_MS);

                gpio_set_level(LED_VERDE_PEDESTRE, 0);
                gpio_set_level(LED_VERMELHO_PEDESTRE, 1);

                gpio_set_level(LED_VERMELHO_CARRO, 0);

               // pedidoPedestre = false;
                estadoAtual = VERDE_CARRO;

            break;
        }
    }
}