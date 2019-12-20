/*
 * menu.h
 *
 *  Created on: 20 Dec 2019
 *      Author: bourc
 */

#ifndef MENU_H_
#define MENU_H_

#include "string.h"
#include "stm32f3xx_hal.h"

#define MAX_CHILDREN 10
#define ADC_RANGE 4096

extern ADC_HandleTypeDef *hadc_global;

typedef struct{
	float value;
	float min_value;
	float max_value;
	char name[16];

}Descriptor;

struct MenuNode{
	char name[16];
	struct MenuNode* parent;
	struct MenuNode* left;
	struct MenuNode* right;
	uint8_t num_children;
	struct MenuNode *children[MAX_CHILDREN];
	void (*command)(void);
	Descriptor* param;
};

struct MenuNode *currentNode;


void BuildMenu(char name[16],void (*command)(void), struct MenuNode **currentNode, struct MenuNode *left);

void InitMenu(void);

void DisplayMenu(void);

#endif /* MENU_H_ */
