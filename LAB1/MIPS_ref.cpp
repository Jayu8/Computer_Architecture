#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
using namespace std;
#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7
#define MemSize 65536 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.


class RF
{
    public:
        bitset<32> ReadData1, ReadData2; 
     	RF()
    	{ 
          Registers.resize(32);  
          Registers[0] = bitset<32> (0);  
        }
	
        void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
        {   
            ReadData1 = Registers[RdReg1.to_ulong()];
            ReadData2 = Registers[RdReg2.to_ulong()];
            if (WrtEnable == 1){
            Registers[WrtReg.to_ulong()] = WrtData;
            }                
         }
		 
	void OutputRF()
             {
               ofstream rfout;
                  rfout.open("RFresult.txt",std::ios_base::app);
                  if (rfout.is_open())
                  {
                    rfout<<"A state of RF:"<<endl;
                  for (int j = 0; j<32; j++)
                      {        
                        rfout << Registers[j]<<endl;
                      }
                     
                  }
                  else cout<<"Unable to open file";
                  rfout.close();
               
               }     
	private:
            vector<bitset<32> >Registers;
	
};

class ALU
{
      public:
             bitset<32> ALUresult;
             bitset<32> ALUOperation (bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2)
             {   
                 switch(ALUOP.to_ulong()){
		    case ADDU:		    
		    ALUresult = bitset<32>(oprand1.to_ulong()+oprand2.to_ulong());
	            //cout << "ALU result - " <<ALUresult <<'\n';
		    break;
		    case SUBU:
    		    ALUresult = bitset<32>(oprand1.to_ulong()-oprand2.to_ulong());
		    break;
		    case AND:
		    ALUresult = oprand1 & oprand2;
		    break;
		    case OR:
		    ALUresult = oprand1 | oprand2;
		    break;
		    case NOR:
		    ALUresult = ~(oprand1|oprand2);
		    break;
            }
                 return ALUresult;
               }            
};

class INSMem
{
      public:
          bitset<32> Instruction;
          INSMem()
          {       IMem.resize(MemSize); 
                  ifstream imem;
                  string line;
                  int i=0;
                  imem.open("imem.txt");
                  if (imem.is_open())
                  {
                  while (getline(imem,line))
                     {      
                        IMem[i] = bitset<8>(line);
                        i++;
                     }
                     
                  }
                  else cout<<"Unable to open file";
                  imem.close();
                     
                  }
                  
          bitset<32> ReadMemory (bitset<32> ReadAddress) 
              {    
               Instruction = bitset<32>((IMem[ReadAddress.to_ulong()].to_ulong()) << 24 | (IMem[ReadAddress.to_ulong()+1].to_ulong()) << 16 | (IMem[ReadAddress.to_ulong()+2].to_ulong()) << 8 | (IMem[ReadAddress.to_ulong()+3].to_ulong()));
               return Instruction;     
              }     
      
      private:
           vector<bitset<8> > IMem;
      
};
      
class DataMem    
{
      public:
          bitset<32> readdata;  
          DataMem()
          {
             DMem.resize(MemSize); 
             ifstream dmem;
                  string line;
                  int i=0;
                  dmem.open("dmem.txt");
                  if (dmem.is_open())
                  {
                  while (getline(dmem,line))
                       {      
                        DMem[i] = bitset<8>(line);
                        i++;
                       }
                  }
                  else cout<<"Unable to open file";
                  dmem.close();
          
          }  
          bitset<32> MemoryAccess (bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem) 
          {    
               
               if (readmem == 1){
               readdata = bitset<32>((DMem[Address.to_ulong()].to_ulong()) << 24 | (DMem[Address.to_ulong()+1].to_ulong()) << 16 | (DMem[Address.to_ulong()+2].to_ulong()) << 8 | (DMem[Address.to_ulong()+3].to_ulong())); 
               }
               
               else if (writemem == 1){
                (DMem[Address.to_ulong()]) = bitset<8> (WriteData.to_string().substr(0,8)); 
                (DMem[Address.to_ulong()+1]) = bitset<8> (WriteData.to_string().substr(8,8));
                (DMem[Address.to_ulong()+2]) = bitset<8> (WriteData.to_string().substr(16,8));
                (DMem[Address.to_ulong()+3]) = bitset<8> (WriteData.to_string().substr(24,8));
               }
               return readdata;     
          }   
                     
          void OutputDataMem()
          {
               ofstream dmemout;
                  dmemout.open("dmemresult.txt");
                  if (dmemout.is_open())
                  {
                  for (int j = 0; j< 1000; j++)
                       {     
                        dmemout << DMem[j]<<endl;
                       }
                     
                  }
                  else cout<<"Unable to open file";
                  dmemout.close();
                    }             

      private:
           vector<bitset<8> > DMem;
      
};  

int main()
{
    INSMem Ins;
    RF rf;
    ALU alu;
    DataMem rd;
    int PC = 0;
    bitset<32> PC4 = bitset<32> (0);
    bitset<32> Instruction;
    bitset<32> readdata;
    bitset<32> WriteData;
    bitset<32> ALUresult;
    bitset<32> Operand1;
    bitset<32> Operand2;
    bitset<30> Branch;
    bitset<32> BranchAdd;
    bitset<32> result;
    bitset<32> store;
    bitset<32> MSB4;
    bitset<32> JumpAdd;

    int i=0;
    while(true){
    Instruction = Ins.ReadMemory(PC);
    if(Instruction==4294967295)
    break;
    else{
    cout << "-------------------------------------" << "\n";
    cout << "Instruction is " << Instruction << "\n";
    bitset<6> opcode(Instruction.to_string().substr(0,6));
    cout << "Opcode is " << opcode << "\n";
    if(opcode == 0){
        cout << "Type is R" << "\n";
        bitset<5> RegRs(Instruction.to_string().substr(6,5));
        bitset<5> RegRt(Instruction.to_string().substr(11,5));
        bitset<5> RegRd(Instruction.to_string().substr(16,5));
        bitset<3> Func(Instruction.to_string().substr(29,3));
        rf.ReadWrite(RegRs, RegRt, RegRd, 0, 0);
        
        Operand1 = rf.ReadData1;
        Operand2 = rf.ReadData2;
        cout << "Operand1 for ALU is " << Operand1 << "\n";
        cout << "Operand2 for ALU is " << Operand2 << "\n";
        ALUresult = alu.ALUOperation(Func, Operand1, Operand2);
        cout << "ALUresult is " << ALUresult << "\n";
        rf.ReadWrite(0, 0, RegRd, ALUresult, 1);
        rf.ReadWrite(RegRd, 0, 0, 0, 0);
        result = rf.ReadData1;
        cout << "Data stored to Reg Rd is " << result << "\n";
    }    
    else if(opcode == 2){
    cout << "J" << "\n";
    PC4 = bitset<32> (PC + 4);
    MSB4 = bitset<32> (PC4.to_string().substr(0, 4)) << 28;
    bitset<32> Imm(Instruction.to_string().substr(6,26));
    bitset<32> Shift(Imm << 2);
    JumpAdd = MSB4 | Shift;
    cout << "Jump to Address "<< JumpAdd << "\n";
    PC = JumpAdd.to_ulong();
    rf.OutputRF();
    continue;
    }    
    else{
    if (opcode == 35){
    cout << "Type is Load" << "\n";
    bitset<5> RegRs(Instruction.to_string().substr(6,5));
    bitset<5> RegRt(Instruction.to_string().substr(11,5));
    cout << "Rt is " << RegRt << "\n";
    rf.ReadWrite(RegRs, 0, 0, 0, 0);
    Operand1 = rf.ReadData1;
    bitset<16> Imm(Instruction.to_string().substr(16,16));
    bitset<1> MSB(Imm.to_string().substr(0,1));
    if (MSB == 0){
    Operand2 = bitset<32>(0+Imm.to_ulong());
    }
    else if (MSB == 1) {
        Operand2 = bitset<32>(1+Imm.to_ulong());
    }
    cout << "Operand1 for ALU is " << Operand1 << "\n"; 
    cout << "Operand2 for ALU is " << Operand2 << "\n";
    ALUresult = alu.ALUOperation(ADDU, Operand1, Operand2);
    cout << "ALUresult is " << ALUresult << "\n";
    readdata = rd.MemoryAccess(ALUresult, 0, 1, 0);
    cout << "Data from Data Memory is " << readdata << "\n";
    rf.ReadWrite(0, 0, RegRt, readdata, 1);
    rf.ReadWrite(0, RegRt, 0, 0, 0);
    result = rf.ReadData2;
    cout << "Data loaded to Register after Load Inst is " << result << "\n";
    }
    else if (opcode == 43){
    cout << "Type is Store" << "\n";
    bitset<5> RegRs(Instruction.to_string().substr(6,5));
    bitset<5> RegRt(Instruction.to_string().substr(11,5));
    cout << "Rt is " << RegRt << "\n";
    rf.ReadWrite(RegRs, 0, 0, 0, 0);
    Operand1 = rf.ReadData1;
    bitset<16> Imm(Instruction.to_string().substr(16,16));
    bitset<1> MSB(Imm.to_string().substr(0,1));
    if (MSB == 0){
    Operand2 = bitset<32>(0+Imm.to_ulong());
    }
    else if (MSB == 1) {
        Operand2 = bitset<32>(1+Imm.to_ulong());
    }
    cout << "Operand1 for ALU is " << Operand1 << "\n"; 
    cout << "Operand2 for ALU is " << Operand2 << "\n";
    ALUresult = alu.ALUOperation(ADDU, Operand1, Operand2);
    cout << "ALUresult is " << ALUresult << "\n";
    rf.ReadWrite(0, RegRt, 0, 0, 0);
    result = rf.ReadData2;
    cout << "Data stored in Reg Rt is " << result << "\n";
    bitset<8> data1(result.to_string().substr(0,8));
    bitset<8> data2(result.to_string().substr(9,8));
    bitset<8> data3(result.to_string().substr(17,8));
    bitset<8> data4(result.to_string().substr(25,8));
    bitset<8> addr1(ALUresult.to_string().substr(0,8));
    rd.MemoryAccess(ALUresult, result, 0, 1);
    rd.MemoryAccess(ALUresult, 0, 1, 0);
    store = rd.readdata;
    cout << store << "\n";
    }
    else if (opcode == 4){
        bitset<5> RegRs(Instruction.to_string().substr(6,5));
        bitset<5> RegRt(Instruction.to_string().substr(11,5));
        rf.ReadWrite(RegRs, RegRt, 0, 0, 0);
	    Operand1 = rf.ReadData1;
	    Operand2 = rf.ReadData2;
	    cout << Operand1 << "\n";
	    cout << Operand2 << "\n";
	    if (Operand1 == Operand2){
	        cout << "It is a beq Instruction" << "\n";
	        bitset<14> Imm(Instruction.to_string().substr(16,14));
            bitset<1> MSB(Imm.to_string().substr(0,1));
            if (MSB == 0){
            Branch = bitset<30>(0+Imm.to_ulong());
            }
            else if (MSB == 1) {
            Branch = bitset<30>(1+Imm.to_ulong());
        }
        cout << Branch << "\n";
            BranchAdd = bitset<32> ((Branch.to_ulong()) << 2);
            cout << BranchAdd << "\n";
            PC = PC + 4 + BranchAdd.to_ulong();
            cout << PC << "\n";
            rf.OutputRF();
            continue;
	    }
    }
    else {
    cout << "I" << "\n";
        bitset<5> RegRs(Instruction.to_string().substr(6,5));
        bitset<5> RegRt(Instruction.to_string().substr(11,5));
        bitset<16> Imm(Instruction.to_string().substr(16,16));
        bitset<1> MSB(Imm.to_string().substr(0,1));
	rf.ReadWrite(RegRs, 0, 0, 0, 0);
	Operand1 = rf.ReadData1;
        if (MSB == 0){
        Operand2 = bitset<32>(0+Imm.to_ulong());
        }
        else if (MSB == 1) {
            Operand2 = bitset<32>(1+Imm.to_ulong());
        }
        cout << "Operand1 for ALU is " << Operand1 << "\n"; 
        cout << "Operand2 for ALU is " << Operand2 << "\n";
        ALUresult = alu.ALUOperation(ADDU, Operand1, Operand2);
        cout << "ALUresult is " << ALUresult << "\n";
        rf.ReadWrite(0, 0, RegRt, ALUresult, 1);
        rf.ReadWrite(RegRt, 0, 0, 0, 0);
        result = rf.ReadData1;
        cout << "Data stored to Reg Rt is " << result << "\n";
    }
    }
    }
    rf.OutputRF();
    PC+=4;
    }
	rd.OutputDataMem();
    return 0;
}
