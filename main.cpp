#include<iostream>
#include<string.h>
#include <conio.h>
#include<iomanip>
using namespace std;
struct podaci{
string korisnickoIme;
string sifra;	
};
void unos(podaci pod[], int &n){
int j=n;
n++;
	for(int i=j;i<n;i++){ // Unosimo podatke 
		cout<<"Unesite korisnicko ime : ";
		cin.ignore();
		getline(cin,pod[i].korisnickoIme);
	for(int j=0;j<i;j++){
		if(pod[j].korisnickoIme==pod[i].korisnickoIme){ 
			do{
				cout<<"Korisnicko ime vec postoji, unesite novo: ";
				getline(cin,pod[i].korisnickoIme);
			}while(pod[j].korisnickoIme==pod[i].korisnickoIme);}	
		}
		
	cout<<"Unesite sifru profila: ";
	char c;
    while ((c = _getch()) != '\r') { // \r je Enter 
    	if (c == '\b' && !pod[i].sifra.empty()) { // brisanje
            pod[i].sifra.pop_back();
        	cout << "\b \b";}
        else if (isprint(c)) {
           pod[i].sifra += c;
        	cout << '*';
        }
    }
    cout<<endl;
}

}
void ispis(podaci pod[], int n){
 for (int i=0;i<n;i++){
 	cout << "Korisnik: " << pod[i].korisnickoIme;
 	cout << "\t\t\tSifra: ";
 	for (int j=0;j<pod[i].sifra.length();j++) cout<< "*";
 	cout<<endl;
 }	
}

void ispisProvjera(podaci pod[], int n){
 for (int i=0;i<n;i++){
 	cout << "Korisnik: " << pod[i].korisnickoIme;
 	cout << setw(2)<<"\t\t\tSifra: " << pod[i].sifra<<endl;
 }	
}
int main(){
int a;
		int brProfila=0;
		podaci pod[100000];
		do{
			cout<<"0. Izlaz"<<endl;
			cout<<"1. Unos profila: "<<endl;
			cout<<"2. Ispis svih profila: "<<endl;
			cout <<"3. Provjera ispisa: "<<endl;
			cout<<endl;
			cout<<"Izaberite opciju: ";
			cin>>a;
			if(a==0){
				cout<<"Izasli ste iz sistema!";
			}
			if(a==1){
				unos(pod,brProfila);
				system("pause");
				system("cls");
			}
			if(a==2){
				ispis(pod,brProfila);
				system("pause");
				system("cls");
			}
			if(a==3){
				ispisProvjera(pod,brProfila);
				system("pause");
				system("cls");
			}

		}
		while(a!=0);
		return 0;	
}
