#include <iostream>
#include <string.h>
#include <conio.h>
#include <iomanip>
#include <fstream>
#include "json/single_include/nlohmann/json.hpp"
#include <openssl/evp.h>
#include <openssl/rand.h>
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
	unsigned char salt[8];
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
//Funckija koja provjerava da li email ima @ 
bool provjeraZnaka (const string email){

	for(int i=0;i<email.length();i++){
		if(email[i]=='@'){
			return true;
		}
	}
		
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

void generisanjeSoli(unsigned char* salt, int duzSalt) {
    RAND_bytes(salt, duzSalt);
}

string hashSifre(const string& sifra, unsigned char* salt, int duzSalt) {
    const int iteracija = 10000; // Broj iteracija salt-a
    const int duzinaKljuca = 32; // SHA-256 napravi 256-bitni kljuc

    unsigned char kljuc[duzinaKljuca]; // Kljuc koji ce se generisati
    PKCS5_PBKDF2_HMAC(sifra.c_str(), sifra.length(), salt, duzSalt, iteracija, EVP_sha256(), duzinaKljuca, kljuc); // Algoritam za generisanje kljuca

    string hash;
    char hexBuf[duzinaKljuca * 2 + 1];
    // Pretvaranje u heksadecimalni zapis
    for (int i = 0; i < duzinaKljuca; i++) {
        sprintf(hexBuf + (i * 2), "%02x", kljuc[i]);
    }
    // Postavljanje null-terminatora
    hexBuf[duzinaKljuca * 2] = '\0';
    // Konverzija u string
    hash = string(hexBuf);

    return hash;
}


void unos() {
	// Unosimo podatke 
	Podaci p;
    json jsonObj;
    int duzinaSifre=0;
	unsigned char salt[8];
    int salt_len = 8;
    cout << "Unesite korisnicko ime : ";
	
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
    
    
    if (!provjeraZnaka(p.email)){
		do{
			cout <<"Vas mail nema @ znak, pokusajte ponovo: ";
			getline(cin,p.email);
		}while(!provjeraZnaka(p.email));
	}
	
    
    
    if (provjeraEmail(p.email, file))
        do {
            cout << "Email vec postoji, unesite ponovo (vas mail mora da sadrzi @ znak): ";
            getline(cin, p.email);
        } while (provjeraEmail(p.email, file) || !provjeraZnaka(p.email) );

	//Predstavljanje sifre preko zvjezdica
    cout << "Unesite sifru profila (minimum 8 karaktera): ";
    char c;
    while ((c = _getch()) != '\r' || duzinaSifre<8) { // \r je Enter 
        if (c == '\b' && !p.sifra.empty()) { // brisanje
            p.sifra.pop_back();
            cout << "\b \b";
            duzinaSifre--;
        }
        else if (isprint(c)) {
            p.sifra += c;
            cout << '*';
            duzinaSifre++;
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
    	cout << "Broj pitanja: ";
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

    generisanjeSoli(p.salt, salt_len); 
    string hashiranaSifra = hashSifre(p.sifra, p.salt, salt_len);

	string hashiranOdgovor=hashSifre(p.odgovor,p.salt,salt_len);
    
	//Pisanje u jedan json objekat
    jsonObj["korisnickoIme"] = p.korisnickoIme;
    jsonObj["email"] = p.email;
    jsonObj["sifra"] = hashiranaSifra;
    jsonObj["sigurnosnoPitanje"] = sigurnosnaPitanja[p.sigurnosnoPitanje - 1];
    jsonObj["odgovor"] = hashiranOdgovor;
    jsonObj["salt"] = p.salt;

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


//Funkcija za promjenu sifre
void promjenaSifre(){
	string temp;
	string odg;
	string novaS;
	string hashOdgovor;
	int duzinaSifre=0;

	ifstream infile(file);
    json jsonNiz;
	
	if (infile.good()) {
        infile >> jsonNiz;
    }
    
	
	cout << "Unesite korisnicko ime: ";
	getline(cin,temp);
	
	if (provjeraKorisnickogImena(temp,file)){ // Provjeravamo da li postoji osoba sa tim korisnickim imenom
		unsigned char salt[8];
	    int salt_len = 8;
		for (auto& jsonObj : jsonNiz){ // prolazimo kroz niz osoba i trazimo isto korisnicko ime
			if(temp==jsonObj["korisnickoIme"]){
				for (int i = 0; i < salt_len; ++i) {
				    salt[i] = jsonObj["salt"][i].get<unsigned char>();
				}
				
				do{	
					cout << "Unesite odgovor za sigurnosno pitanje! " <<endl;
					cout << jsonObj["sigurnosnoPitanje"] << endl;
					getline(cin,odg);
					hashOdgovor=hashSifre(odg,salt,salt_len);
				}while(jsonObj["odgovor"]!=hashOdgovor);
				
				cout << "Unesite novu sifru profila (minimum 8 karaktera): ";
				char c;
				while ((c = _getch()) != '\r' || duzinaSifre<8) { // \r je Enter 
	    			if (c == '\b' && !novaS.empty()) { // brisanje
						novaS.pop_back();
			        	cout << "\b \b";
			        	duzinaSifre--;
					}
					else if (isprint(c)) {
						novaS += c;
			        	cout << '*';
			        	duzinaSifre++;
					}
				}	
			
			generisanjeSoli(salt, salt_len);
	    	string hashiranaSifra = hashSifre(novaS, salt, salt_len);
	    	string novihashOdg=hashSifre(odg,salt,salt_len);
			jsonObj["sifra"]=hashiranaSifra;
			jsonObj["odgovor"]=novihashOdg;
			jsonObj["salt"] = salt; 
			ofstream outfile(file);
			
			cout<<endl;
			outfile << jsonNiz.dump(4) << endl; // zapis u file sa novom sifrom
		    outfile.close();
		 
			return;
			}
		}

}
	cout << "Nismo pronasli korisnika!"<<endl;
}



//Funkcija za ulaz u sistem
void login(){
	
	string username;
	string password;
	unsigned char salt[8];
    int salt_len = 8;
	ifstream infile(file);
    json jsonNiz;
	
	if (infile.good()) {
        infile >> jsonNiz;
    }
    
	cout << "Unesite korisnicko ime: ";
	getline(cin,username);
	if (provjeraKorisnickogImena(username,file)){ //Provjerava da li postoji korinsik sa tim korisnickim imenom
		for (auto& jsonObj : jsonNiz){ // prolazi kroz niz
			if(username==jsonObj["korisnickoIme"]){ // staje na osobi koja ima isto korisnicko ime
				for (int i = 0; i < salt_len; ++i) {
				    salt[i] = jsonObj["salt"][i].get<unsigned char>();
				}
				int br=0;
				string hashedPassword = "";
				do {
					password.erase();	// u slucaju da osoba unese sifru 2 ili vise puta , brisemo prethodno unesene podatke
					cout << "Unesite sifru profila: ";
					char c;
					while ((c = _getch()) != '\r') { // \r je Enter 
				    	if (c == '\b' && !password.empty()) { // brisanje
							password.pop_back();
				        	cout << "\b \b";
						}
						else if (isprint(c)) {
							password += c;
				        	cout << '*';
						}
					}
					cout<<endl;	
					hashedPassword = hashSifre(password, salt, salt_len);
					br++;
				}while(jsonObj["sifra"]!=hashedPassword && br!=3); // Unosi sifru sve dok ne unese tacnu sifru
				if(br==3){
					cout << "PREVISE PUTA STE POGRIJESILI SIFRU, POKUSAJTE KASNIJE\n";
					return;
				} 
				cout<<"--------------------------------"<<endl;
				cout << "USPJESNO STE USLI U SISTEM! "<<endl;
				return;
			}
		}
	}
	else {
		cout << "Korisnik nije pronadjen u sistemu! "<<endl;
		
	}
}

void brisanjeKorisnika(){
	
	ifstream infile(file);
    json jsonNiz;
     
    if (infile.good()) {
        infile >> jsonNiz;
    }
    int index=0;
	string username;
	cout << "Unesite korisnicko ime za brisanje: ";
	getline(cin,username);
	if(provjeraKorisnickogImena(username,file)){
		for (auto& jsonObj : jsonNiz){index++; 
			if(username==jsonObj["korisnickoIme"]){	
				jsonNiz.erase(index-1);
				ofstream outfile(file);
				cout<<endl;
				outfile << jsonNiz.dump(4) << endl; 
		    	outfile.close();
		    	return;
			}
		}
	}
	else{
		cout << "Osoba ne postoji sa tim korisnickim imenom! "<<endl;
	}
	
}

void ispisKorisnika(){
	
	ifstream infile(file);
    json jsonNiz;
     
    if (infile.good()) {
        infile >> jsonNiz;
    }
    
    cout<<"Unesite korisnicko ime osobe cije podatke zelite ispisati: ";
    string username;
    getline(cin,username);
    
    if(provjeraKorisnickogImena(username,file)){
    	for(auto &jsonObj:jsonNiz){
    		if(jsonObj["korisnickoIme"]==username){
    			cout << "Korisnicko ime: " << jsonObj["korisnickoIme"] << endl;
        		cout << "Email: " << jsonObj["email"] << endl;
        		cout<< "Sifra: ";
        		string sifra=jsonObj["sifra"];
				for(int i=0;i<sifra.length();i++){
        			cout<<"*";
				}
				cout<<endl;
        		cout << "Sigurnosno pitanje: " << jsonObj["sigurnosnoPitanje"] << endl;
        		cout << "Odgovor: " << jsonObj["odgovor"] <<endl;
        		cout << endl;
        		infile.close();
        		return;
			}
		}
	}
	else{
		cout << "Nismo pronasli korisnika! ";
	}
    	
	
	
}


void adminMenu(){
	int k;	
	bool fail, uspjeh = false;
	string username;
	string password;
	unsigned char salt[8];
    int salt_len = 8;
	ifstream infile(file);
    json jsonNiz;
	
	if (infile.good()) {
        infile >> jsonNiz;
    }
    
	cout << "Unesite korisnicko ime: ";
	getline(cin,username);
	if(username != "Ivaylo"){
		cout << "Admin ne postoji!\n";
		return;
	}
	for (auto& jsonObj : jsonNiz){ // prolazi kroz niz
		if(username==jsonObj["korisnickoIme"]){ // staje na osobi koja ima isto korisnicko ime
			for (int i = 0; i < salt_len; ++i) {
			    salt[i] = jsonObj["salt"][i].get<unsigned char>();
			    
			}
			int br=0;
			string hashedPassword = "";
			do {
				password.erase();	// u slucaju da osoba unese sifru 2 ili vise puta , brisemo prethodno unesene podatke
				cout << "Unesite sifru profila: ";
				char c;
				while ((c = _getch()) != '\r') { // \r je Enter 
			    	if (c == '\b' && !password.empty()) { // brisanje
						password.pop_back();
			        	cout << "\b \b";
					}
					else if (isprint(c)) {
						password += c;
			        	cout << '*';
					}
				}
				cout<<endl;	
				hashedPassword = hashSifre(password, salt, salt_len);
				br++;
			}while(jsonObj["sifra"]!=hashedPassword && br!=3); // Unosi sifru sve dok ne unese tacnu sifru
			if(br==3){
				cout << "PREVISE PUTA STE POGRIJESILI SIFRU, POKUSAJTE KASNIJE\n";
				return;
			} 
			cout<<"--------------------------------"<<endl;
			cout << "USPJESNO STE USLI U SISTEM! "<<endl;
			uspjeh = true;
		}
	}
	if(uspjeh){
		do {  	
			system("cls");
			cout<<"1. Ispis svih profila "<<endl;
			cout<<"2. Provjera ispisa "<<endl;
			cout<<"3. Brisanje profila " <<endl;
			cout<<"4. Pretraga po korisnickom imenu"<<endl;
			cout<<"0. Izlaz"<<endl;
			cout<<endl;
				do{
				
		      		cout<<"Izaberite opciju: "<<endl; 
		      		cin >> k;  
					fail = cin.fail();  
					cin.clear();  
		      		cin.ignore(INT_MAX, '\n');  
	  			}while(fail);
			if (k==0){
				return;
			}
			if(k==1){
				ispis();
			}
			if(k==2){
				ispisProvjera();
				
			}
			if (k==3){
				brisanjeKorisnika();
			}	
			if(k==4){
				ispisKorisnika();
			}
			system("pause");
			system("cls");	
		
		}while(true);	
	}
	else
		cout << "GRESKA!\n";
	
}

int main(){

	int a;
	bool fail;
	do{
		cout<<"0. Izlaz"<<endl;
		cout<<"1. Unos profila "<<endl;
		cout<<"2. Promjena sifre " <<endl;
		cout<<"3. Login"<<endl;
		cout<<"4. Admin"<<endl;
		cout<<endl;
		do  
   		{  
	      	cout<<"Izaberite opciju: "; 
	      	cin >> a;  
			fail = cin.fail();  
			cin.clear();  
	      	cin.ignore(INT_MAX, '\n');  
  		}while(fail);  
		
		if(a==0){
			cout<<"Izasli ste iz sistema!";
			return 0;
		}
		if(a==1){
			unos();
		}
		if(a==2){
			promjenaSifre();
			
		}
		if(a==3){
			login();			
		}
		if (a==4){
			adminMenu();
		}
		system("pause");
		system("cls");
		

	}while(true);
	
}

