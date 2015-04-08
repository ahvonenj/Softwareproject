# Softwareproject

JAMK / Olio ohjelmointi 2 lopputyö.

## 1. Projektin kuvaus

Päätimme tehdä Olio-2 projektiksi pienimuotoisen pelin, jossa olisi tarkoitus selviytyä laavakentällä mahdollisimman kauan.

Laava lähtee leviämään kentän laidoilta, ja pelaaja pystyy vesipyssyllä muuttamaan laavatiilejä takaisin kivitiiliksi.

Pelaajan kuoltua ennemmin tai myöhemmin suoritus pisteytetään sammutettujen tiilien sekä selvityn ajan mukaan.

Mukana pelissä olisi myös alkuvalikko, highscoret sekä mahdollisesti muita ominaisuuksia.

## 2. Projektin ominaisuudet

* Ohjelmointikieli:	C/C++
* Kirjastot:	SFML multimediakirjasto
* Koodirivejä C++:	~3800
* Koodirivejä PHP:	~250
* Koodirivejä yhteensä:	~4000

## 3. Käytännön toteutus

Valitsimme pelin toteuttamista varten pienen mietinnän jälkeen SFML-kirjaston, koska siitä oli molemmilla ryhmän jäsenillä jo hieman aiempaa kokemusta, ja se oli todettu toimivaksi ratkaisuksi.

Sovelluksen kehityksen elinkaaren loppupuolella kehittelimme MASA-ohjelmistokehityskehyksen (vrt. Scrum, DSDM). MASA-kehys (Mieskolainen Agile Software Assertion) yhdistää monien muiden ketterien ohjelmistokehitysmenetelmien parhaita puolia huipputehokkaaksi kokonaisuudeksi.

MASA :aa kehittäessä haimme paljon inspiraatiota Jyväskylän Ammattikorkeakoulun lehtorin Matti Mieskolaisen innovatiivisestä ja mullistavasta otteesta opettamiseen, joten kiitos siitä hänelle.

MASA voidaan kiteyttää lyhyesti seuraavaan loogiseen lauseeseen:
Hyvin suunniteltu = Puoliksi tehty => Puoliksi tehty = Hyvin suunniteltu

Selkokielellä jos Hyvin suunniteltu on equivelantti Puoliksi tehdyn kanssa, niin sen seurauksena Puoliksi tehty equivelantti Hyvin suunnitellun kanssa.

## 4. Luokat

### 4.1 Game

Itse pelin pääluokka on Game.

Game ottaa parametreikseen osoittimen piirtoikkunaan, sekä charin, jonka avulla Game tietää, pyöritetäänkö täysinäistä versiota, vai graafisesti karsittua muunnosta.

Teimme karsitun version siksi että koulun koneet eivät jaksaneet enää pyörittää peliä 60 FPS.

Tämä johtuu siitä, että OpenGL Shading Languge, jota on käytetty laava-shaderissä, pyörii näytönohjaimella.

Koulun koneiden näytönohjaimet sattuvat olemaan kehnoja, tai sitten tarvittavia ajureita ei ole päivitetty.

Game-luokan tärkein metodi on Run.

Kun peli on käynnissä, suoritetaan aluksi pelin logiikan käsittely tapahtumien prosessointimetodeilla (esim. ProcessEvents), sitten päivitämme pelin logiikkaa (Update-metodit) ja lopuksi piirrämme näytölle pelin tilan render-metodissa.

Tämä kaikki suoritetaan 60 kertaa sekunnissa, jotta saadaan sujuva ja pelattava loppukokonaisuus.

Tämän perusrakenteen Game-luokan jakamisesta prosessointiin, päivittämiseen sekä piirtämiseen saimme SFML:n kehittäjän, Laurent Gomilian kirjasta SFML Game Development (ISBN: 1849696845, julkaisukuukausi 6.2013.)

### 4.2 World

World-luokka on itse pelikenttä.

Annamme sille osoittimina/viittauksina piirtoikkunan, tekstuurimanagerin, pelaajan,vesiprojektiilimanagerin, äänimanagerin, sekä scoren.

Worldissa hoidamme tärkeitä toimintoja kuten collision detectionin pelaajan, laavan sekä vesiprojektiilien välillä, sekä laavan leviämisen.

Worldin ylivoimaisesti tärkein metodi on update, jossa kaikki edellämainitut asiat tapahtumat. Se on rivimäärissä mitattuna sovelluksen isoin metodi.

### 4.3 Scene

Scene-luokka on sekä pelin että päävalikon kantaluokka, joka sisältää vain yhden virtuaalisen metodin, Runin.

Run palauttaa kokonaisluvun, jonka avulla pystymme hyppimään eri tapahtumista toiseen pelin main-metodissa.

Tämä mahdollistaa helposti uusien kokonaisuuksien lisäämisen peliin periyttämällä ne scenestä, esimerkiksi creditsit tai moninpeli olisi suhteellisen helppo lisätä.

Totta kai pelin eri tiloilla on muitakin samankaltaisuuksia kuin Run esimerkiksi piirtofunktio ja tapahtumienkäsittely, mutta emme nähneet käytännön kannalta tarpeelliseksi periyttää näitä abstraktista luokasta.

### 4.4 Player

Player-luokka kuvaa pelin pelaajaa, kuten nimestä voikin päätellä. 

Pelaajalla on muunmuassa nopeus, kiihtyvyys, hitpointsit ja ammukset. 

Pelaajan tärkeimmät metodit ovat liikkumisesta vastaava calculateVelocity(), ampumisesta vastaava shoot(), sekä pelaajan päivittämisestä pelimaailmassa vastaava update().

### 4.5 Entity

Entityjä ovat pelin objektit, kuten pelaajan projektiilit ja Powerupit.

Kaikille Entityille yhteistä on lähinnä elinaikaan liittyvät muuttujat/metodit.

Entityjä hallitaan puolestaan EntityManager-luokalla, joka on Entity-tyyppisen vektorin ympärille rakennettu wrapperi.

Entitymanager huolehtii Entityjen lisäämisestä ja kuolleiden Entityjen pois siivoamisesta

### 4.6 Score

Score-luokka pitää, yllätys yllätys, huolta pelaajien pisteistä ja niiden yleisestä hallinnasta kuten resetoinnista, lisäämisestä ja loppupisteiden laskemisesta.
Tämän lisäksi Score luokka hoitaa scorejen hashauksen ja lähetyksen webbiin sekä POST-replyn vastaanottamisen ja käsittelyn.

### 4.7 Menu

Menu on oikeastaan aika pieni, mutta silti kompleksi luokka.
Menu-luokka huolehtii päävalikon toiminnasta, versioncheckistä ja osittain myös pelin uudelleenkäynnistämisestä.

Tunnelman luomiseksi menuun on koodattu fade-in ja fade-out efektit, eli menu siis tulee esiin asteittan ja häipyy samalla tavalla.

### 4.8 Entitymanager

Entitymanager-luokka pitää sisällään entityvektorin ja hallitsee niitä.

Entitymanager on template-luokka ja näin ollen sama luokka voi pitää huolta erityyppisistä entityistä tai peliobjekteista.

Entitymanagerin ehkäpä tärkein metodi on clean-metodi, joka käy läpi oman vektorinsa kaikki entityt ja poistaa kuolleeksi merkatut entityt pelistä viemästä muistia.
### 4.9 Muut luokat

Muita vähäpätöisempiä(kö?) luokkia ovat Tile, Powerup ja Ammopowerup.

Nämä luokat ovat tietysti tärkeä osa kokonaisuutta, mutta samalla myös tylsiä ja melko tyhjiä, periaatteessa apuluokkia.

Sitten on vielä kaikki resurssimanagerit kuten Texturemanager ja Sfxmanager, joiden tehtävänä on pitää huolta että peli lataa käynnistyessään tarvitut kuvat ja äänet ja pitää ne muistissaan sulkemiseen asti.

## 5. Tietovarastot

Pelissä käytetyt resurssit (tekstuurit, ääniefektit) ladataan omiin manageriluokkiinsa, joista niitä voidaan hakea ID:n perusteella.

Näin kehittäjän ei tarvitse muistaa ulkoa resurssien tiedostopolkuja tai tyyppejä tms.

Toinen pelissä käyttämämme tietovarasto on Score-luokka, joka pelaajan kuoltua ottaa talteen pelaajan saaman pistemäärän sekä nimimerkin, ja lähettää ne Labranetillä pyörivälle palvelimelle POST-viestinä.

Ennen lähettämistä nimi puhdistetaan mahdollisista haitallisista merkeistä (esim. |, $), ja palvelinpäässä se tallennetaan yksinkertaiseen taulukkoon, jossa pelaaja voi vertailla kuinka on pärjännyt.

## Koodista

Koodi on mitä se on ja sitä voi käydä lueskelemassa..
Esimerkiksi World-luokassa käytettiin paljon luuppeja ja muuta logiikkaroinaa, jotta laava saadaan leviämään hienosti ja optimoidusti, joten senkin läpikäyminen on paitsi aikaa vievää että yleisen ohjelmoinnin kannalta turhaa..

Peli ja luokat on rakennettu idealla "Game > World > Player", eli Game-luokka hoitaa suurimman osan asioista, sitten World, sitten Player.

Pidämme Game-luokkaa pelin niinsanottuna pääluokkana josta miltein kaikki lähtee ja johon miltein kaikki päätyy.

Onko tällainen ajattelutapa sitten olio-ohjelmoinnin kannalta sen parempi tai huonompi kuin mikään muukaan, niin vastauksena siihen että ainakin peli pyörii ja kaiken lisäksi vielä melkoisen hyvällä suorituskyvyllä ja muistin käytöllä.

Pelin teossa tavoitteena oli pääasiassa uusien asioiden keksiminen ja kokeileminen ja tässä onnistuttiin mitä loistavimmin.

## 7. Pelistä

### 7.1 Pelikuvaa

Seuraavassa videossa esitellään millainen softwareproject on pelinä.
Peliä pelataan normaaleilla asetuksilla ja parhaimmilla grafiikoilla.

https://youtu.be/cy7d45uMZv4

### 7.2 Highscoret

Softwareprojectin highscoret tallennetaan HTTP POST-requestilla web-palvelimelle, josta niitä voi selata web-käyttöliittymällä.

Kyseisen sivuston voi löytää esimerkiksi tästä linkistä.

![](https://github.com/ahvonenj/Softwareproject/blob/master/github_resources/highscores.JPG?raw=true)

### 7.3 Moninpeli

Moninpeli on viimeisimpiä lisäyksiä Softwareprojektiin.
Moninpelissä peliin lisätään toinen pelaaja, jota on mahdollista ohjata esimerkiksi xbox-ohjaimella.

![](https://github.com/ahvonenj/Softwareproject/blob/master/github_resources/multiplayer.JPG?raw=true)

## 8. PHP:n käyttö

Softwareproject käyttää muutamaa eri PHP-skriptiä.

Pisteiden "kuuntelu"- ja tallennus-skripti parsettaa ja validoi Softwareproject.exen sille lähettämät pisteidenlisäys-requestit.
Mikäli lisäyspyyntö ei jostain syystä mene läpi, lähettää tämä skripti siitä replyn takaisin Softwareproject.exelle.

Pisteiden lataus-skripti järjestää ja tulostaa web-sivulle sen hetkiset highscoret.

Versioncheck-skripti kertoo pyydettäessä Softwareproject.exelle nykyisen uusimman version hashin.
Mikäli Softwareproject.exe on vanhentunut sille lähetetään latauslinkki uusimpaan versioon palvelimella.

## 9. Luokkakaavio

![](https://github.com/ahvonenj/Softwareproject/blob/master/github_resources/luokkakaavio.png?raw=true)

## 10. Mainittavia bugeja / ongelmia

Vaikkakin ohjelmistoprojektissa tulee aina ongelmia ja bugeja vastaan, niin tässä on muutama suurin:

Tilejen spritet katosivat muistista satunnaisesti.
Tämä bugi johti texturemenagerin ja kaikkien muiden managerien suunnitteluun ja toteutukseen.

Väärä merkistökoodaus joissain tiedostoissa.
Tämä ongelma oli erityisen ärsyttävä sillä koodi oli oikein, mutta esimerkiksi C++:n tietyt funktiot eivät ymmärtäneet ANSI-merkistökoodausta ja näin ollen lukivat mitä sattuu.

Joitain aikaongelmia.
Nämä ongelmat liittyivät pelin päivitysnopeuteen ja tapahtumien kulkuun. Välillä jokin asia tapahtui hetkessä vaikka sen olisi pitänyt odottaa 15 sekuntia.

Virustorjunta.
Kiitos Avastin "loistavan" virustentorjunnan, se ei antanut välillä ajaa koko ohjelmaa ollenkaan. Allekirjoittanut onkin nyt siirtynyt käyttämään MSE.

Teemun POST-request kaappaukset ja spooffaus.
Tämä ongelma johti highscorejemme lähetysten suojaamiseen ylimääräisellä hashilla, jonka luonnin ja ns. purkamisen näkee vain plaintext-lähdekoodista

## 11. Itsearvio

Kaiken kaikkiaan olemme sitä mieltä että peli onnistui hyvin.

Toki aina leikkauspöydälle jää joitain ominaisuuksia joiden toteuttaminen katsottiin käytännössä olevan tarpeetonta, mutta silti pelistä tuli jokseenkin sellainen kuin sen aluksi olimme visualisoineet.

Joitakin ominaisuuksia emme edes osanneet alussa pohtia varteenotettavina, esimerkiksi scoren tallentaminen palvelimelle.

Puutteina kehitystyössä tulee päällimmäisenä mieleen kunnollisen projektinhallinnan puute.

Esimerkiksi GitHubin avulla ei tarvitsisi erikseen selvittää mitä muutettiin/lisättiin ja suorittaa sitten muutosten yhdistys, vaan kehitysprosessi nopeutuisi huomattavasti.

Onneksi peli olikin loppujen lopuksi vain 4000 riviä koodia, ja kahden ohjelmoijan kesken sen manageroimisessa ei esiintynyt huomattavia ongelmia.

## Työn kesto

1,5 kuukautta

## Tekijät

* Jonah Ahvonen
* Matti Jokitulppo

## Lisenssijuttuja

Vastuuvapauslauseke: Kaikki lainatut resurssit on otettu vain opetuskäyttöä varten.

* Ohjelmointi: Jonah Ahvonen & Matti Jokitulppo.
* SFML multimedia library: Laurent Gomila (sfml-dev.org).
* Taustamusiikki: NES Silver Surfer (c) NINTENDO / Software Creations / Arcadia Systems.
* Ääniefektit: SFXR.
* Tile texturet: opengameart.org.
* Pelaajasprite: Dushan Chaciej | ANtY (everyday-im-drawing.blogspot.fi).
* Lava shader: glsl.heroku.com GLSL Sandbox.
* Palomiesko naama: (c) M. Mieskolainen.
* 04b.ttf fontti: dafont.com.
* Water droplet sprite: rock1111100 (piq.codeus.net).
* C++ md5 algoritmi: http://www.zedwood.com/article/cpp-md5-function
* C++ IniReader ja IniWriter: http://www.codeproject.com/Articles/10809/A-Small-Class-to-Read-INI-File

## Muuta

Kiitos testaajille!

