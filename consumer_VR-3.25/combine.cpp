#include "combine.h"

using namespace svcdash;

int Combine::combine(string outFile,string initFile,vector<string>& segFile) {    
    Py_Initialize();   //初始化 
     
    PyRun_SimpleString("import sys");   
    PyRun_SimpleString("sys.path.append('./')");    

   // import Module
    PyObject * pName = PyString_FromString("merge");
    if(!pName){
        cout<<pName<<endl;
        cout<<"error"<<endl;
        return -1;
    }
    else{
        cout<<pName<<endl;
        cout<<"success"<<endl;
    }
    PyObject * pModule = PyImport_Import(pName);
    
    if (!pModule) {    
        cout<<"Can't import Module!"<<endl;    
        return -1;    
    }    
    else {    
        cout<<pModule<<endl;
        cout<<" import Module!"<<endl;    
        
    } 

    PyObject * pFunc = PyObject_GetAttrString(pModule,"mux");

    if(!pFunc || !PyCallable_Check(pFunc))
    {
        printf("Can't find function[mux]");
        getchar();
        return -1;
    }
    else{
        cout<<pFunc<<endl;
        cout <<"import func!"<<endl;
    }
    PyObject *pArgs = NULL;
    pArgs = PyTuple_New(3);
    PyTuple_SetItem(pArgs,0,Py_BuildValue("s",outFile.c_str()));
    PyTuple_SetItem(pArgs,1,Py_BuildValue("s",initFile.c_str()));

    switch(segFile.size()){
        case 1:
        {
            PyTuple_SetItem(pArgs,2,Py_BuildValue("[s]",segFile[0].c_str()));
            break;
        }
        case 2:
        {                       
            PyTuple_SetItem(pArgs,2,Py_BuildValue("[s,s]",segFile[0].c_str(),segFile[1].c_str()));
            break;
        }
        case 3:
        {                       
            PyTuple_SetItem(pArgs,2,Py_BuildValue("[s,s,s]",segFile[0].c_str(),segFile[1].c_str(),segFile[2].c_str()));
            break;
        }
        case 4:
        {                       
            PyTuple_SetItem(pArgs,2,Py_BuildValue("[s,s,s,s]",segFile[0].c_str(),segFile[1].c_str(),segFile[2].c_str(),segFile[3].c_str()));
            break;
        }
        case 5:
        {                       
            PyTuple_SetItem(pArgs,2,Py_BuildValue("[s,s,s,s]",segFile[0].c_str(),segFile[1].c_str(),segFile[2].c_str(),segFile[3].c_str(),segFile[4].c_str()));
            break;
        }
	case 6:
        {                       
            PyTuple_SetItem(pArgs,2,Py_BuildValue("[s,s,s,s]",segFile[0].c_str(),segFile[1].c_str(),segFile[2].c_str(),segFile[3].c_str(),segFile[4].c_str(),segFile[5].c_str()));
            break;
        }
	case 7:
        {                       
            PyTuple_SetItem(pArgs,2,Py_BuildValue("[s,s,s,s]",segFile[0].c_str(),segFile[1].c_str(),segFile[2].c_str(),segFile[3].c_str(),segFile[4].c_str(),segFile[5].c_str(),segFile[6].c_str()));
            break;
        }
	case 8:
        {                       
            PyTuple_SetItem(pArgs,2,Py_BuildValue("[s,s,s,s]",segFile[0].c_str(),segFile[1].c_str(),segFile[2].c_str(),segFile[3].c_str(),segFile[4].c_str(),segFile[5].c_str(),segFile[6].c_str(),segFile[7].c_str()));
            break;
        }
	case 9:
        {                       
            PyTuple_SetItem(pArgs,2,Py_BuildValue("[s,s,s,s]",segFile[0].c_str(),segFile[1].c_str(),segFile[2].c_str(),segFile[3].c_str(),segFile[4].c_str(),segFile[5].c_str(),segFile[6].c_str(),segFile[7].c_str(),segFile[8].c_str()));
            break;
        }
	case 10:
        {                       
            PyTuple_SetItem(pArgs,2,Py_BuildValue("[s,s,s,s]",segFile[0].c_str(),segFile[1].c_str(),segFile[2].c_str(),segFile[3].c_str(),segFile[4].c_str(),segFile[5].c_str(),segFile[6].c_str(),segFile[7].c_str(),segFile[8].c_str(),segFile[9].c_str()));
        }
    }

    PyObject_CallObject(pFunc,pArgs);
    cout<<outFile<<" combine finished!"<<endl;
    
    Py_Finalize();    
    return 0;    
}


string Combine::merge(string initFile,vector<string>& segFile){
	//create output filename
	string outFile;
	string temp=segFile[0];
	int offset=temp.find("-L",0);
	outFile=temp.substr(0,offset)+".264";
	//cout<<"@@@@@@@@@@"<<outFile<<endl;
	combine(outFile,initFile,segFile);
	return outFile;
}

/*
int main(){
    string outFile1="segment1.svc";
    string outFile2="segment2.svc";
    string outFile3="segment3.svc";
    string outFile4="segment4.svc";
    string initFile="TOS-I-360p.init.svc";
    string baseFile="TOS-I-360p.seg0-L0.svc";
    string enhanFile1="TOS-I-360p.seg0-L1.svc";
    string enhanFile2="TOS-I-360p.seg0-L2.svc";
    string enhanFile3="TOS-I-360p.seg0-L3.svc";
    clock_t start,end;
    vector<string> segFile;
    segFile.push_back(baseFile);
    segFile.push_back(enhanFile1);
    segFile.push_back(enhanFile2);
    segFile.push_back(enhanFile3);
    //combine(outFile1,initFile,segFile);
    start=clock();
    combine(outFile4,initFile,segFile);
    end=clock();
    cout<<(double)(end-start)/CLOCKS_PER_SEC<<endl;
	
}*/

