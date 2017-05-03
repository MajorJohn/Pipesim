#include "simulator.h"

using namespace std;

Pipesim::Pipesim()
{
	//number of lines
	int _iLines = 0;

	string line;
	ifstream myfile ("code.txt");

	if (myfile.is_open())
	{
		//discover how many line the code has
		while(!myfile.eof()) 
		{
			getline (myfile, line);

			_iLines++;
		}

		//reopen the file
		myfile.close();
		myfile.open ("code.txt");

		//save the parameters
		_nLines = _iLines;
		_sCode = new string[_iLines];
		_tCode = new tuple[_iLines];

		//save the lines of the code
		_iLines = 0;
		while(!myfile.eof()) 
		{
			getline (myfile, line);

			_sCode[_iLines] = line;
			_iLines++;
		}

		myfile.close();

		translate();
		//printM();
	}

	else cout << "Unable open the file\n";
}

Pipesim::~Pipesim()
{
	delete [] _sCode;
	delete [] _tCode;
}

void //DONE
Pipesim::translate()
{
	for (int i = 0; i < _nLines; ++i)
	{
		tin(i);
	}
}

void //DONE
Pipesim::tin(int _iLine)
{
	//case is a Label
	if(_sCode[_iLine][_sCode[_iLine].length() - 1] == ':')
	{
		_tCode[_iLine].type = 6;
		saveLabel(_iLine);
	}

	//case is a add or sub
	else if (_sCode[_iLine][0] == 'a' ||
			(_sCode[_iLine][0] == 's' && _sCode[_iLine][1] == 'u'))
	{ 
		_tCode[_iLine].type = 1;
		tinr(_iLine);
	}

	//case is a beq or bnq
	else if (_sCode[_iLine][0] == 'b')
	{
		_tCode[_iLine].type = 2;
		tinr(_iLine);
		jLabel(_iLine);
	}

	//case is a jump
	else if (_sCode[_iLine][0] == 'j')
	{
		_tCode[_iLine].type = 3;
		jLabel(_iLine);
	}

	// case is a lw
	else if (_sCode[_iLine][0] == 'l')
	{
		_tCode[_iLine].type = 4;
		tinr(_iLine);
	}

	//case is a sw
	else if (_sCode[_iLine][0] == 's')
	{
		_tCode[_iLine].type = 5;
		tinr(_iLine);
	}
}

void //DONE
Pipesim::tinr(int _iLine)
{
	int index = 2;
	int nRegister = 1; // number of the register
	int trans = 0; // translate

	while (_sCode[_iLine][index] != '\0')
	{
		if(_sCode[_iLine][index] == '$')
		{
			index++;
			//case $zero
			if(_sCode[_iLine][index] == 'z'){
				trans = 0;
			}
			else{

				//case $tx
				if(_sCode[_iLine][index] == 't')
					trans = 10;
			
				//case $sx
				else
					trans = 20;

				//add the 'x' of $tx / $sx
				index++;
				trans += (_sCode[_iLine][index]-48);
			}

			//case the operation is add or sub
			if (_tCode[_iLine].type == 1)
			{
				if(nRegister == 1)
				{
					_tCode[_iLine].rBlock = trans;
				}
				else if(nRegister == 2)
				{
					_tCode[_iLine].rNeed1 = trans;
				}
				else
				{
					_tCode[_iLine].rNeed2 = trans;
				}
			}

			// case is a beq or bne
			else if (_tCode[_iLine].type == 2)
			{
				if(nRegister == 1)
				{
					_tCode[_iLine].rNeed1 = trans;
				}
				else
				{
					_tCode[_iLine].rNeed2 = trans;
				}
			}

			// case is a lw
			else if (_tCode[_iLine].type == 4)
			{
				if(nRegister == 1)
				{
					_tCode[_iLine].rBlock = trans;
				}
				else
				{
					_tCode[_iLine].rNeed1 = trans;
				}
			}

			// case is a sw
			else if (_tCode[_iLine].type == 5)
			{
				if(nRegister == 1)
				{
					_tCode[_iLine].rNeed1 = trans;
				}
				else
				{
					_tCode[_iLine].rNeed2 = trans;
				}
			}

			nRegister++;
		}

		index++;
	}
}

void //DONE
Pipesim::saveLabel(int _iLine)
{
	string _sLabel = _sCode[_iLine];

	int i = 1;

	while (_sLabel[_sCode[_iLine].length() - i] != ':')
	{
		i++;
	}

	_sLabel.resize(_sCode[_iLine].length() - i);

	_tCode[_iLine].label = _sLabel;

	_mLabels[_sLabel] = _iLine;
}

void //DONE
Pipesim::jLabel(int _iLine)
{
	string _sLabel = _sCode[_iLine];

	unsigned int i = 1;
	unsigned int j = 0;

	unsigned int sz;

	while (_sLabel[_sCode[_iLine].length() - i] != ' ')
	{
		i++;
	}


	i--;
	sz = i;
	
	do{
		_sLabel[j] = _sLabel[_sCode[_iLine].length() - i];

		i--;
		j++;
	} while (_sLabel[_sCode[_iLine].length() - i] != '\0');

	_sLabel.resize(sz);

	_tCode[_iLine].label = _sLabel;
}

void //DONE
Pipesim::printM()
{
	for (int i = 0; i < _nLines; ++i)
	{
		if(i+1 >= 10)
			cout << ">>> " << i +1 << " - ";
		else
			cout << ">>> " << i +1 << "  - ";

		cout << _tCode[i].type << " ";

		if (_tCode[i].type == 1)
		{
			cout << _tCode[i].rBlock << " ";
			cout << _tCode[i].rNeed1 << " ";
			cout << _tCode[i].rNeed2;			
		}
		else if (_tCode[i].type == 2)
		{
			cout << _tCode[i].rNeed1 << " ";
			cout << _tCode[i].rNeed2 << " ";
			cout << _tCode[i].label;
		}
		else if (_tCode[i].type == 3)
		{
			cout << _tCode[i].label;
		}
		else if (_tCode[i].type == 4)
		{
			cout << _tCode[i].rBlock << " ";
			cout << _tCode[i].rNeed1;
		}
		else if (_tCode[i].type == 5)
		{
			cout << _tCode[i].rNeed1 << " ";
			cout << _tCode[i].rNeed2;
		}
		else if (_tCode[i].type == 6)
		{
			cout << _tCode[i].label;
		}

		cout << "\n";
	}
}

void //DONE
Pipesim::simCollisions()
{

	_iPipeline[0] = -1;
	_iPipeline[1] = -1;
	_iPipeline[2] = -1;
	_iPipeline[3] = -1;
	_iPipeline[4] = -1;

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			_mbFreeRegisters[i][j] = true;
		}
	}

	_iPipeline[0] = 0;
	_iCycle++;

	while(_iPipeline[0] < _nLines -1)
	{
		printStage();
		updatePipe();
		_iCycle++;

		//printR();
	}

	while(	_iPipeline[0] != -1 ||
			_iPipeline[1] != -1 ||
			_iPipeline[2] != -1 ||
			_iPipeline[3] != -1 ||
			_iPipeline[4] != -1
		 )
	{
		printStage();
		updatePipe();
		_iCycle++;
	}
	cout << "-------------------------------------------------------\n";
}

void //DONE
Pipesim::printStage()
{
	cout << "-------------------------------------------------------\n";
	cout << "Cycle: " << _iCycle << "\n";


	cout << "IF:	";
	if(_iPipeline[0] != -1)
		cout << _sCode[_iPipeline[0]] << endl;
	else
		cout << 0 << endl;


	cout << "ID:	";
	if(_iPipeline[1] != -1)
		cout << _sCode[_iPipeline[1]] << endl;
	else
		cout << 0 << endl;


	cout << "EX:	";
	if(_iPipeline[2] != -1)
		cout << _sCode[_iPipeline[2]] << endl;
	else
		cout << 0 << endl;


	cout << "MEM:	";
	if(_iPipeline[3] != -1)
		cout << _sCode[_iPipeline[3]] << endl;
	else
		cout << 0 << endl;

	cout << "WB:	";
	if(_iPipeline[4] != -1)
		cout << _sCode[_iPipeline[4]] << endl;
	else
		cout << 0 << endl;
}

void //DONE
Pipesim::addCollision(int _iLine)
{
	int i;
	int j;

	i = _tCode[_iLine].rBlock / 10 -1;
	j = _tCode[_iLine].rBlock % 10;
	
	//WARNING!!!
	//I don't know why, but here is changing the _iPipeline[3]
	//in the test code
	_mbFreeRegisters[i][j] = false;
	
	
}

void //DONE
Pipesim::freeCollision(int _iLine)
{
	int i;
	int j;

	i = _tCode[_iLine].rBlock / 10 -1;
	j = _tCode[_iLine].rBlock % 10;

	_mbFreeRegisters[i][j] = true;
}

void //DONE
Pipesim::updatePipe()
{
	
	if(	_iPipeline[4] != -1 &&
		_tCode[_iPipeline[4]].type != 2 &&
		_tCode[_iPipeline[4]].type != 3 &&
		_tCode[_iPipeline[4]].type != 5 
		)
		freeCollision(_iPipeline[4]);


	for (int i = 4; i > 1; --i)
	{
		_iPipeline[i] = _iPipeline[i - 1];
	}

	_iPipeline[1] = -1;


	// a trick to run the program
	int vec[5];

	for (int i = 0; i < 5; ++i)
		vec[i] = _iPipeline[i];




	if(_iPipeline[0] == -1) return;
	
	if(	verifyCollision(_iPipeline[0]))
	{
		addCollision(_iPipeline[0]);
		
		for (int i = 0; i < 5; ++i)
			_iPipeline[i] = vec[i];

		_iPipeline[1] = _iPipeline[0];

		
		//the number of the line of the next instruction
		int _iNextEnter = _iPipeline[0] + 1;

		//case it is a line of label
		if(_tCode[_iNextEnter].type == 6)
			_iNextEnter++;

		//has a jump
		if (_tCode[_iPipeline[1]].type == 2 ||
			_tCode[_iPipeline[1]].type == 3)
			_iNextEnter = _mLabels[_tCode[_iPipeline[1]].label] +1;

		//added the intruction in the pipeline
		if(_iNextEnter >= _nLines)
			_iPipeline[0] = -1;
		else
			_iPipeline[0] = _iNextEnter;

	}

}


bool //DONE
Pipesim::verifyCollision(int _iLine)
{
	if( _tCode[_iLine].type == 3 )
		return true;

	int i;
	int j;

	//every intruction that need two parameters
	if( _tCode[_iLine].type == 1 ||
		_tCode[_iLine].type == 2 ||
		_tCode[_iLine].type == 5
		)
	{
		//case register needed 1 is diferent of $zero
		if(_tCode[_iLine].rNeed1 != 0){

			i = _tCode[_iLine].rNeed1 / 10 -1;
			j = _tCode[_iLine].rNeed1 % 10;

			if(!_mbFreeRegisters[i][j])
				return false;
		}

		if(_tCode[_iLine].rNeed2 != 0){

			i = _tCode[_iLine].rNeed2 / 10 -1;
			j = _tCode[_iLine].rNeed2 % 10;

			if(!_mbFreeRegisters[i][j])
				return false;
		}
	}
	else if (_tCode[_iLine].type == 4)
	{
		if(_tCode[_iLine].rNeed1 != 0){

			i = _tCode[_iLine].rNeed1 / 10 -1;
			j = _tCode[_iLine].rNeed1 % 10;

			if(!_mbFreeRegisters[i][j])
				return false;
		}
	}



	return true;
}

void //DONE
Pipesim::printR()
{
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			if(i == 0)
				cout << "t" << j << " ";
			else
				cout << "s" << j << " ";
			
			if(_mbFreeRegisters[i][j])
				cout << "free -- ";
			else 
				cout << "blocked -- ";
		}
		cout << "\n";
	}
}

void //DONE
Pipesim::printPipe()
{
	for (int i = 0; i <5; ++i)
	{
		cout << _iPipeline[i] << " ";
	}
	cout << "\n";
}
