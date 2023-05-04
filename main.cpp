#include <iostream>
#include <string.h>
#include <conio.h>
#include <iomanip>
#include <fstream>
#include "json/single_include/nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

const string sigurnosnaPitanja[] = { "Koji vam je omiljeni film?", "Kako se zove vas najdrazi nastavnik?", "Koja vam je omiljena boja?", "Koja vam je omiljena hrana?", "Kako se zove ulica na kojoj ste odrasli?" };
//Fajl koji cemo korsistiti kao bazu
const string file = "podaci.txt";

//Struktura za podatke

struct Podaci{
	string korisnickoIme;
	string sifra;	
	string email;
	int sigurnosnoPitanje;
	string odgovor;
};

//Funkcija koja provjerava da li korisnicko ime vec postoji
bool provjeraKorisnickogImena(const string& korisnickoIme, const string& file)
{
	ifstream infile(file);
    json jsonNiz;
    
    //Provjerava da li je infile u dobrom stanju (tj. da li je bilo gresaka pri ucitavanju), a zatim ga ucitava u niz JSON blokova
    if (infile.good()) {
        infile >> jsonNiz;
    }
    
    //Iterira kroz niz i provjerava da li korisnicko ime postoji
    for (const auto& jsonObj : jsonNiz) {
        if(jsonObj["korisnickoIme"] == korisnickoIme)	
        	return true;
    }
    
    infile.close();
    return false;
}

//Funkcija koja provjerava da li email vec postoji
bool provjeraEmail(const string& email, const string& file)
{
	ifstream infile(file);
    json jsonNiz;
    
    //Provjerava da li je infile u dobrom stanju (tj. da li je bilo gresaka pri ucitavanju), a zatim ga ucitava u niz JSON blokova
    if (infile.good()) {
        infile >> jsonNiz;
    }
    
    //Iterira kroz niz i provjerava da li email postoji
    for (const auto& jsonObj : jsonNiz) {
        if(jsonObj["email"] == email)	
        	return true;
    }
    
    infile.close();
    return false;
}

void unos() {
	// Unosimo podatke 
	Podaci p;
    json jsonObj;
    cout << "Unesite korisnicko ime : ";
    cin.ignore();
    getline(cin, p.korisnickoIme);
    //Ponavlja se unos ako se unese vec postojece korisnicko ime
    if (provjeraKorisnickogImena(p.korisnickoIme, file))
        do {
            cout << "Korisnicko ime vec postoji, unesite ponovo: ";
            getline(cin, p.korisnickoIme);
        } while (provjeraKorisnickogImena(p.korisnickoIme, file));

	//Ponavlja se unos ako se unese vec postojeci email
    cout << "Unesite email: ";
    getline(cin, p.email);
    if (provjeraEmail(p.email, file))
        do {
            cout << "Email vec postoji, unesite ponovo: ";
            getline(cin, p.email);
        } while (provjeraEmail(p.email, file));

	//Predstavljanje sifre preko zvjezdica
    cout << "Unesite sifru profila: ";
    char c;
    while ((c = _getch()) != '\r') { // \r je Enter 
        if (c == '\b' && !p.sifra.empty()) { // brisanje
            p.sifra.pop_back();
            cout << "\b \b";
        }
        else if (isprint(c)) {
            p.sifra += c;
            cout << '*';
        }
    }
    cout << endl;
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
    } while (p.sifra != potvrdaSifre);
    cout << endl;
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

    ifstream infile(file);
    json jsonNiz;
    if (infile.good()) {
        infile >> jsonNiz;
    }
	//Pisanje u jedan json objekat
    jsonObj["korisnickoIme"] = p.korisnickoIme;
    jsonObj["email"] = p.email;
    jsonObj["sifra"] = p.sifra;
    jsonObj["sigurnosnoPitanje"] = sigurnosnaPitanja[p.sigurnosnoPitanje - 1];
    jsonObj["odgovor"] = p.odgovor;

	//dodavanje json objekta na kraj json niza
    jsonNiz.push_back(jsonObj);
	
	//pisanje niza u fajl
    ofstream outfile(file);
    outfile << jsonNiz.dump(4) << endl;
    outfile.close();

	cout<<endl;
}

//Funkcija za ispis
void ispis() {
    ifstream infile(file);
    json jsonNiz;
    
    //Provjerava da li je infile u dobrom stanju (tj. da li je bilo gresaka pri ucitavanju), a zatim ga ucitava u niz JSON blokova
    if (infile.good()) {
        infile >> jsonNiz;
    }
    
    //Iteracija kroz json niz i ispis odredjenih parametara
    for (const auto& jsonObj : jsonNiz) {
        cout << "Korisnicko ime: " << jsonObj["korisnickoIme"] << endl;
        cout << "Email: " << jsonObj["email"] << endl;
        string sifra = jsonObj["sifra"];
        cout << "Sifra: ";
        for(int i=0; i<sifra.length(); i++) cout << "*";
        cout << endl << endl;
    }
    
    infile.close();
}

//Funkcija za testiranje i provjeru
void ispisProvjera(){
	ifstream infile(file);
	json jsonNiz;
	
	if(infile.good()){
		infile >> jsonNiz;
	}
	
	for (const auto& jsonObj : jsonNiz) {
        cout << "Korisnicko ime: " << jsonObj["korisnickoIme"] << endl;
        cout << "Email: " << jsonObj["email"] << endl;
        cout << "Sifra: " << jsonObj["sifra"] << endl;
        cout << "Sigurnosno pitanje: " << jsonObj["sigurnosnoPitanje"] << endl;
        cout << "Odgovor: " << jsonObj["odgovor"] <<endl;
        cout << endl;
    }
    infile.close();
}

int main(){

	int a;

	//Menu
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
			unos();
			system("pause");
			system("cls");
		}
		if(a==2){
			ispis();
			system("pause");
			system("cls");
		}
		if(a==3){
			ispisProvjera();
			system("pause");
			system("cls");
		}

	}while(a!=0);

	return 0;	
}

