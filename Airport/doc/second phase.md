# Project Airport
Naš projekt je aplikáciou pre letisko, ktoré bude pracovať na strane zamestnanca letiska.
## 1.Scenár - pridanije určitého letu
Aby určitý let "Fly" bol pridaný je potrebné uviesť "Gate", "Pláne", "Flight" a "Dáte"
Pred pridaniem letu je možné pridať "Gate", "Pláne", "Flight" samostatne(tak isto ako "Passenger"). 
Pre každé pridanie vytvorené nezávisle menu. 
### Použitie:
#### 1)Pridanie Gate
	-vybrať možnosť pridania Gate v "Add data menu"
	-v novom okne zadať meno Gate a stlačiť "Apply"
#### 2)Pridanie  Pláne
	-vybrať možnosť pridania Plane v "Add data menu"
	-v novom okne zadať nutné dáta a stlačiť "Apply"
#### 3)Pridanie  Flight
	-vybrať možnosť pridania Flight v "Add data menu"
	-v novom okne zadať nutné dáta a stlačiť "Apply"	
#### 4)Pridanie  Passenger
	-vybrať možnosť pridania Passenger v "Add data menu"
	-v novom okne zadať nutné dáta a stlačiť "Apply"	
#### 4)Pridanie  Fly
	-vybrať možnosť pridania Fly v "Add data menu"
	-v novom okne zadať dátum letu
	-vedľa poľa "Flight" stlačiť "Search"
		-v novom okne zadať kritériá podľa ktorých treba vyhľadať existujúce lety(Napríklad v "from" stačí napísať "Bra" a sa zobrazia všetky lety ktoré v stlbce "from" na začiatku majú "Bra") a stlačiť "Search"
		-vybrať jeden z variantov v tabuľke a stlačiť "Apply"(id vybraného elementu sa zobrazí v predchádzajúcom okne).
	-podobne treba vybrať Gate a Pláne
	-stlačiť "Apply" a nový "Fly" je v database
### Implementácia
Pre pracu z databazou bole vytvorene 2 metody:

	-public void addData(String table, String columns, String values)
	-public ResultSet getData(String table, String where)

#### addData()
Na vstup dostava:

	-String table ktory musi mat tvar "nazov_tabulky"
	-String columns ktory musi mat tvar "(prvy,druhy,tretí)" 
	-String values ktory musi mat tvar "'slovo',1,'text'" 
Na zaklade vstupu metoda spoji to vsetko do jedneho riadku 
######		String query = "INSERT INTO " + table + " " + columns + " VALUES (" + values + ")";
A zavola 
###### 		executeUpdate(query);

#### getData()
Na vstup dostava:

	-String table ktory musi mat tvar "nazov_tabulky"
	-String where ktory musi mat tvar "'column1 LIKE 'text%' AND column2 = 2'" 
Na zaklade vstupu metoda spoji to vsetko do jedneho riadku 
######		String query = "SELECT * FROM " + table + " WHERE " + where;
A zavola 
###### 		ResultSet result = stmt.executeQuery(query);
result potom vráti pre spracovanie v "Controller"
	

## 2.Scenár - pridanie batožiny
Ak chcete pridať novú batožinu, vyberte v hlavnom menu položku „Edit baggage“.
### Použitie:
#### 1)Search
		-v novom okne zadať id "Boarding pass", ku ktorému je potrebné pridať batožinu a stlačiť "Search"
#### 1)Add baggage
		-v novom okne zadať hmotnosť batožiny a stlačiť "ADD"
### Implementácia
Pre pracu z databazou bol vytvorena metoda:
		
		-public void addDataTransaction(String table, String columns, String values, String transaction)
Bol reazovany s použitím transakcií. Ak nastane chyba pri pridávaní batožiny, cena sa nezmení.

#### addDataTransaction()
Na vstup dostava:

		-String table ktory musi mat tvar "nazov_tabulky"
		-String columns ktory musi mat tvar "(prvy,druhy,tretí)" 
		-String values ktory musi mat tvar "'slovo',1,'text'" 
		-String transaction ktory musi mat tvar "; UPDATE public.boardingpass SET price = price + 30.00 WHERE id_boarding_pass = 1"
		-String transaction sa sklada zo všetkych možnych requestov
Na zaklade vstupu metoda spoji to vsetko do jedneho riadku
######	 String query = "BEGIN; DELETE FROM " + table + " WHERE " + columns + "= " + values + "; " + transaction + "; COMMIT;";
A zavola 
###### 		executeUpdate(query);





## 3.Scenár - odstránenie batožiny
Ak chcete odstraniť batožinu, vyberte v hlavnom menu položku „Edit baggage“.
### Použitie:
#### 1)Search
		-v novom okne zadať id "Boarding pass", od ktoreho je potrebné odstraniť batožinu a stlačiť "Search"
		-ak je to hľadany Boarding pass stlačiť "Apply"
#### 1)Delete baggage
		-v novom okne vybrať batožinu ktoru je potrebné odstraniť a stlačiť "Delete"
### Implementácia
Pre pracu z databazou bol vytvorena metoda:
		
		-public void deleteDataTransaction(String table, String columns, String values, String transaction)
Bol reazovany s použitím transakcií. Ak dôjde k chybe pri odstraňovaní batožiny, cena sa nezmení.

#### deleteDataTransaction()
Na vstup dostava:

		-String table ktory musi mat tvar "nazov_tabulky"
		-String columns ktory musi mat tvar "(prvy,druhy,tretí)" 
		-String values ktory musi mat tvar "'slovo',1,'text'" 
		-String transaction ktory musi mat tvar "; UPDATE public.boardingpass SET price = price - 30.00 WHERE id_boarding_pass = 1"
		-String transaction sa sklada zo všetkych možnych requestov 
Na zaklade vstupu metoda spoji to vsetko do jedneho riadku
######	 String query = "BEGIN; DELETE FROM " + table + " WHERE " + columns + "= " + values + "; " + transaction + "; COMMIT;";
A zavola 
###### 		executeUpdate(query);
