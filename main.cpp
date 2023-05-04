#include <iostream>
#include <string.h>
#include <conio.h>
#include <iomanip>
#include <vector>

using namespace std;

const string sigurnosnaPitanja[] = { "Koji vam je omiljeni film?", "Kako se zove vas najdrazi nastavnik?", "Koja vam je omiljena boja?", "Koja vam je omiljena hrana?", "Kako se zove ulica na kojoj ste odrasli?" };

struct Podaci{
	string korisnickoIme;
	string sifra;	
	string email;
	int sigurnosnoPitanje;
	string odgovor;
};

void unos(vector<Podaci> &pod){
	// Unosimo podatke 
	Podaci p;
	cout<<"Unesite korisnicko ime : ";
	cin.ignore();
	getline(cin,p.korisnickoIme);
	for(int i=0;i<pod.size();i++){
		if(p.korisnickoIme==pod[i].korisnickoIme){ 
			do{
			cout<<"Korisnicko ime vec postoji, unesite novo: ";
			getline(cin,p.korisnickoIme);
			}while(p.korisnickoIme==pod[i].korisnickoIme);
		}	
	}
	cout<<"Unesite email: ";
	getline(cin, p.email);
	for (int i = 0;i < pod.size();i++) {
		if (p.email == pod[i].email) {
			do {
				cout << "Korisnicko ime vec postoji, unesite novo: ";
				getline(cin, p.email);
			} while (p.email == pod[i].email);
		}
	}
	cout<<"Unesite sifru profila: ";
	char c;
	while ((c = _getch()) != '\r') { // \r je Enter 
    	if (c == '\b' && !p.sifra.empty()) { // brisanje
			p.sifra.pop_back();
        	cout << "\b \b";}
		else if (isprint(c)) {
			p.sifra += c;
        	cout << '*';
		}
	}
	cout<<endl;
	cout << "Potvrdite sifru profila: ";
	string potvrdaSifre;
	c = '\0';
	do {
		while ((c = _getch()) != '\r') { // \r je Enter 
			if (c == '\b' && !potvrdaSifre.empty()) { // brisanje
				potvrdaSifre.pop_back();
				cout << "\b \b";
			}
			else if (isprint(c)) {
				potvrdaSifre += c;
				cout << '*';
			}
		}
	}while(p.sifra != potvrdaSifre);
	cout<<endl;
	cout << "Odaberite sigurnosno pitanje: " << endl;
	for (int i = 0; i < 5; i++) {
		cout << i + 1 << ". " << sigurnosnaPitanja[i] << endl;
	}
	do {
		cin >> p.sigurnosnoPitanje;
	} while (p.sigurnosnoPitanje < 1 || p.sigurnosnoPitanje > 5);
	cout << "Unesite odgovor na sigurnosno pitanje: ";
	cin.ignore();
	getline(cin, p.odgovor);
	pod.push_back(p);
	cout<<endl;
}

void ispis(vector<Podaci> &pod){
	 for (int i=0;i<pod.size();i++){
 		cout << "Korisnik: " << pod[i].korisnickoIme;
		cout << "\t\tE-mail: " << pod[i].email;
 		cout << "\t\tSifra: ";
 		for (int j=0;j<pod[i].sifra.length();j++) cout<< "*";
 		cout<<endl;
	 }	
}

void ispisProvjera(vector<Podaci> &pod){
	 for (int i=0;i<pod.size();i++){
 		cout << "Korisnik: " << pod[i].korisnickoIme;
		cout << "\t\tE-mail: " << pod[i].email;
 		cout << setw(2)<<"\tSifra: " << pod[i].sifra << endl;
		cout << "Sigurnosno pitanje: " << sigurnosnaPitanja[pod[i].sigurnosnoPitanje - 1];
		cout << "\tOdgovor: " << pod[i].odgovor << endl;
	 }	
}
int main(){
		int a;
		vector<Podaci> pod;
		do{
			cout<<"0. Izlaz"<<endl;
			cout<<"1. Unos profila "<<endl;
			cout<<"2. Ispis svih profila "<<endl;
			cout<<"3. Provjera ispisa "<<endl;
			cout<<endl;
			cout<<"Izaberite opciju: ";
			cin>>a;
			if(a==0){
				cout<<"Izasli ste iz sistema!";
			}
			if(a==1){
				unos(pod);
				system("pause");
				system("cls");
			}
			if(a==2){
				ispis(pod);
				system("pause");
				system("cls");
			}
			if(a==3){
				ispisProvjera(pod);
				system("pause");
				system("cls");
			}

		}while(a!=0);

		return 0;	
}