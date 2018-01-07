#define _GLIBCXX_USE_C99 1
#include<iostream>
#include<string>
#include<vector>
#include<stdlib.h>
#include<fstream>
#include<cmath>
#include<sstream>
#include<bitset>
#include<iomanip>
#include<cstdlib>
#include<cstring>


int bin2dec(const char* str) 
{
 int n = 0;
 int size = strlen(str) - 1;
        int count = 0;
 while ( *str != '\0' ) {
  if ( *str == '1' ) 
      n = n + pow(2, size - count );
  count++; 
  str++;
 }
 return n;
}


void print_matrix(int a[],int INSTRUCTION_SIZE)
{
	for(int i=0; i < INSTRUCTION_SIZE; i++)
	{
		std::cout << a[i] << std::endl;
	}
}



    int** Array_2D(unsigned hght, unsigned wdth)
    {
      int** matrix2D = 0;
      matrix2D = new int*[hght];

      for (int h = 0; h < hght; h++)
      {
            matrix2D[h] = new int[wdth];

            for (int w = 0; w < wdth; w++)
            {
                  
                  matrix2D[h][w] = 1;
            }
      }

      return matrix2D;
    }


int main()
{
	//************************READING THE CONFIG FILE***************
	
	std::ifstream config_file("config.txt");
	int  m=0,k;
	std::string config_line;
	while(std::getline(config_file,config_line))
	{
		std::istringstream config_iss(config_line);
		if (config_iss && m !=0)
		{
			config_iss >> k;	
		}
		if (config_iss && m ==0)
		{
			config_iss >> m;	
		} 

	}
	printf("The value of M is %d and K is %d\n", m , k );
	
	//**************************READING PC, ACTUAL BRANCH TAKEN AND SLICING***********************************//
	std::ifstream trace_file_no("trace.txt");
	std::string trace_line_no;
	int inst=0;
	while(std::getline(trace_file_no,trace_line_no))
	{
		std::istringstream trace_iss_no(trace_line_no);
		if (trace_iss_no)
		{
			inst++; 
		}
		 
	}
	int INSTRUCTION_SIZE = inst; // NUMBER OF INSTRUCTION
	std::cout << "THE Number of instructions are : " << INSTRUCTION_SIZE << std::endl;


	//DECLARATIONS
	std::string a[INSTRUCTION_SIZE];
	int         branch[INSTRUCTION_SIZE];

	int PC[INSTRUCTION_SIZE];
	unsigned long  temp;

	std::string A[INSTRUCTION_SIZE];
  	std::bitset<20> LSBs;

	std::ifstream trace_file("trace.txt");
	std::string trace_line;
	int i=0;
	while(std::getline(trace_file,trace_line))
	{
		std::istringstream trace_iss(trace_line);
		if (trace_iss)
		{
			trace_iss >> a[i] >> branch[i];	
			//std::cout << a[i] << " "<< branch[i] << "\n";
			i++; 
		}
		 
	}
	// Here code is converting the numbers into 'a' string matrix and 'b' bitset matrix

	
	//Here the string matrix is converted to a bitset matrix and later it is being split for 5 hex numbers(m=20).
	int value;
	int thevalue;	

	std::string brr;
	for(int i=0; i < INSTRUCTION_SIZE; i++)
	{
		std::stringstream str;
		
		brr = a[i].substr(3,7);
		str << brr;
		str >> std::hex >> value;
		std::string binary = std::bitset<20>(value).to_string();


		
		//LSBs = std::bitset<20>(binary.substr(0,20)); 
		LSBs = std::bitset<20>(binary);
		//std::cout<<value<<" "<<LSBs<<std::endl;
		A[i] =  binary.substr(20-m,20);  
		//std::cout << i << std::endl;
		
		std::string str1 = A[i];
		const char* bin_str = str1.c_str();
		PC[i] = bin2dec(bin_str);


		//PC[i] = std::stoi(A[i], nullptr, 2);
		//std::cout << PC[i] << std::endl;		
		 
	}

    
//***************************************************DECALRING ARRAYS FOR LOGIC ***********************************//

	// Concatenating Saturating counters into a single big matrix  , depends on size m and k
	  int height = pow(2,m);
      int width  = 2*pow(2,k);
      int** SaturatingCounter = Array_2D(height, width);
      
    // Bufer history register , depends on size k
      int BHR[k];
      for (int i = 0; i < k; ++i)
      {
      	BHR[i] =1;
      	
      }

//***************************************************  LOGIC ***********************************//
      int COUNT =0; 
      int Output[INSTRUCTION_SIZE];
      int INDEX1;
      int INDEX2;

      for(i = 0; i < INSTRUCTION_SIZE;i++)
      {
      		 Output[i] = 0;

      }
	

      while(COUNT<INSTRUCTION_SIZE)
      {  
	     
	      int SUM=0;
	      for (int i = 0; i < k; ++i)
	      {
	      	SUM += BHR[i] * pow(2,i);
	      }
	      INDEX2 = 2*SUM;

	      INDEX1 = PC[COUNT];

	    
	      if ((SaturatingCounter[INDEX1][INDEX2] == 1) && (SaturatingCounter[INDEX1][INDEX2+1] == 1) && (branch[COUNT] == 1)) //11
	      {
	      		
	      		
	      		Output[COUNT]=1;                            // Trace file output                              
	      		SaturatingCounter[INDEX1][INDEX2] = 1;       //Strong State
	      		SaturatingCounter[INDEX1][INDEX2+1] = 1;     //Strong State
		  }

	      else if ((SaturatingCounter[INDEX1][INDEX2] == 1) && (SaturatingCounter[INDEX1][INDEX2+1] == 0) && (branch[COUNT] == 1)) //10
	      {
	      		
	      		 
	      		Output[COUNT]=1;                             // Trace file output                              
	      		SaturatingCounter[INDEX1][INDEX2] = 1;       //Strong State
	      		SaturatingCounter[INDEX1][INDEX2+1] = 1;     //Strong State
		  }

	      else if ((SaturatingCounter[INDEX1][INDEX2] == 0) && (SaturatingCounter[INDEX1][INDEX2+1] == 0) && (branch[COUNT] == 1)) //00
	      {
	      		 
	      		Output[COUNT]=0;                             // Trace file output                              
	      		SaturatingCounter[INDEX1][INDEX2] = 0;       //Weak State
	      		SaturatingCounter[INDEX1][INDEX2+1] = 1;     //Strong State


	      }

		  else if ((SaturatingCounter[INDEX1][INDEX2] == 0) && (SaturatingCounter[INDEX1][INDEX2+1] == 1) && (branch[COUNT] == 1)) //01
	      {
	      		
	      		Output[COUNT]=0;                             // Trace file output                              
	      		SaturatingCounter[INDEX1][INDEX2] = 1;       //Strong State
	      		SaturatingCounter[INDEX1][INDEX2+1] = 1;     //Strong State


	      }

          ////////////////////////////******************NOT TAKEN ******************///////////////////////

	      else if ((SaturatingCounter[INDEX1][INDEX2] == 1) && (SaturatingCounter[INDEX1][INDEX2+1] == 1) && (branch[COUNT] == 0)) //11
	      {
	      		 
	      		Output[COUNT]=1;                                
	      		SaturatingCounter[INDEX1][INDEX2] = 1;       //Strong State
	      		SaturatingCounter[INDEX1][INDEX2+1] = 0;     //Weak State


	      }
	      

	      else if ((SaturatingCounter[INDEX1][INDEX2] == 1) && (SaturatingCounter[INDEX1][INDEX2+1] == 0) && (branch[COUNT] == 0)) //10
	      {
	      		
	      		Output[COUNT]=1;                               
	      		SaturatingCounter[INDEX1][INDEX2] = 0;       //Weak State
	      		SaturatingCounter[INDEX1][INDEX2+1] = 0;     //Weak State
		  }

	      else if ((SaturatingCounter[INDEX1][INDEX2] == 0) && (SaturatingCounter[INDEX1][INDEX2+1] == 0) && (branch[COUNT] == 0)) //00
	      {
	      		 
	      		Output[COUNT]=0;                             // Trace file output                              
	      		SaturatingCounter[INDEX1][INDEX2] = 0;       //Weak State
	      		SaturatingCounter[INDEX1][INDEX2+1] = 0;     //Weak State


	      }
		  else if ((SaturatingCounter[INDEX1][INDEX2] == 0) && (SaturatingCounter[INDEX1][INDEX2+1] == 1) && (branch[COUNT] == 0)) //01
	      {
	      		 
	      		Output[COUNT]=0;                             // Trace file output                              
	      		SaturatingCounter[INDEX1][INDEX2] = 0;       //Weak State
	      		SaturatingCounter[INDEX1][INDEX2+1] = 0;     //Weak State


	      }
	    
	      for (int i = k-1; i > 0; --i)
	      {
	      	BHR[i]=BHR[i-1];
	      }
	      
	      BHR[0]=branch[COUNT];
	      COUNT++;    	

      }	
    
      //print_matrix(Output,INSTRUCTION_SIZE);
	 
	           
      std::ofstream branch_res;
      branch_res.open("trace.txt.out",std::ios_base::app);
      if (branch_res.is_open())
      {
        //branch_res<<"A state of RF:"<<std::endl;
        for (int j = 0; j<INSTRUCTION_SIZE; j++)
        {        
            branch_res << Output[j]<<std::endl;
        }
                 
      }
      else std::cout<<"Unable to open file";
            branch_res.close(); 



	return 0;
}
