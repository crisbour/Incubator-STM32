/*
 * menu.c
 *
 *  Created on: 20 Dec 2019
 *      Author: bourc
 */

#include "menu.h"
#include "stdlib.h"
#include "lcd1602.h"
#include "string.h"

void initMenuNode(struct MenuNode*);

void BuildMenu(char name[16],void (*command)(void), struct MenuNode **current_Node, struct MenuNode *left){
	//Setup the currentNode
	struct MenuNode *Node=malloc(sizeof(*Node));
	initMenuNode(Node);
	strcpy(Node->name,name);
	Node->left=left;
	if(left!=0)
		Node->left->right=Node;

	Node->command=command;


	//Update currentNode to the children list of the parent
	(*current_Node)->children[(*current_Node)->num_children]=Node;
	(*current_Node)->num_children ++;

	Node->parent=currentNode;
	uint8_t offset_Instruction_Register=0;

	free(Node);

	(*current_Node)=(*current_Node)->children[(*current_Node)->num_children-1];


}
void BackMenu(struct MenuNode *back){
	back->children[0]=back->parent;
	back->num_children=1;
}

uint16_t readADC(){
	HAL_ADC_Start(hadc_global);
	HAL_ADC_PollForConversion(hadc_global, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(hadc_global);
}


struct MenuNode* RightMostChildren(struct MenuNode* Node){
	if(Node->num_children==0)	return 0;
	return Node->children[Node->num_children - 1];
}
void initMenuNode(struct MenuNode* Node){
	for(uint8_t i=0;i<MAX_CHILDREN;i++)
		Node->children[i]=NULL;
	for(uint8_t i=0;i<16;i++)
		Node->name[i]=NULL;
	Node->num_children=0;
	Node->left=NULL;
	Node->right=NULL;
	Node->parent=NULL;
}
void InitMenu(void){
	//Initialize God parent
	currentNode=(struct MenuNode*)malloc(sizeof(currentNode));
	initMenuNode(currentNode);
	//Start from the God parent
	BuildMenu("Temperature",NULL, &currentNode, RightMostChildren(currentNode));	//the currentNode becomes the children Node just initialized

	currentNode=currentNode->parent;											//go back to the parent
	BuildMenu("Control",NULL,&currentNode, RightMostChildren(currentNode));//create next children Node
																				//and so on
	currentNode=currentNode->parent;
	BuildMenu("Time",NULL,&currentNode, RightMostChildren(currentNode));


}

void menuIndex(struct MenuNode **currentNode){
	static uint8_t index=0;

	struct MenuNode *parent=malloc(sizeof(*parent));
	parent=(*currentNode)->parent;

	uint8_t num_menus=parent->num_children;
	uint16_t tol=ADC_RANGE/num_menus/8;


	uint16_t adc_val=readADC();
	if(((index+1)*ADC_RANGE/num_menus+tol)<adc_val)
		index++;

	if((index*ADC_RANGE/num_menus-tol)>adc_val)
		index--;

	*currentNode=parent->children[index];
}

void DisplayMenu(void){
	menuIndex(&currentNode);
	LCD_setCursor(0,1);
	LCD_printstr(currentNode->name);
}
