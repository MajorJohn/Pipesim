#ifndef _SIMULATOR_H
#define _SIMULATOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;



class Pipesim{

	private:

		struct tuple {
			int type = 0;
			int rBlock = 0; // register using as result
			int rNeed1 = 0; // register using as parameter
			int rNeed2 = 0; // register using as parameter
			string label; // if use, it keeps the name of the label
		};

	
		map<string, int> _mLabels; //map that store the labels
		string * _sCode; //lines of the code
		int _nLines; //number of lines
		tuple * _tCode; // matrix that keeps the code in numbers
		
		// has the line in process for each slot
		// if it is -1 in the slot, there is nothing in that moment
		int _iPipeline[5];
		int _iCycle = 0;


		//if the register is free to use
		//ex.:
		//		is $t9 free?
		//		_bFreeRegisters[1 - 1][9] == true (1-1 cause the code to $tx is 1x)
		//		so, it's free
		bool _mbFreeRegisters[2][9];
		

	//*******************************************************
	//************ PRIVATE FUNCTIONS ********************************
	//*******************************************************



		//translate the code
		void translate();

		//transform in numbers
		//input: a int as parameter, n line of the code
		//code the parameter to numbers
		//****************************
		//		add/sub = 1
		//		beq/bne = 2
		//		   j    = 3
		//		  lw    = 4
		//		  sw    = 5
		//		 LABEL  = 6
		//****************************
		void tin(int _iLine);

		//transform in numbers register
		//input: a int as parameter, n line of the code
		//code the parameter to numbers
		//ps.: used only when needed (jump and LABEL don't need)
		//****************************
		//	       $tx  = 1x
		//	       $sx  = 2x
		//	      $zero = 0
		//****************************
		void tinr(int _iLine);

		//save the LABEL in a map and the respective line
		//input: a int as parameter, n line of the code
		void saveLabel(int _iLine);

		//save the LABEL to jump
		//input: a int as parameter, n line of the code
		void jLabel(int _iLine);

		//print matrix
		void printM();

		//add the registers blocked in the matrix _bFreeRegisters
		void addCollision(int _iLine);

		//free the registers blocked in the matrix _bFreeRegisters
		void freeCollision(int _iLine);

		//print the stage of the pipeline
		void printStage();

		//verify if the line has a collision
		bool verifyCollision(int _iLine);


		//update the pipeline
		//if a new one enters, return true
		void updatePipe();

		//print register using or not
		void printR();

		//print the numbers in the pipeline
		void printPipe();


	public:

		//constructor without parameters
		Pipesim ();
		//destructor
		~Pipesim();

		//simulate the colisions and store in a vector
		void simCollisions();
};

#endif
