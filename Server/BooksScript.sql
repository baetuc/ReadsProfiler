create table creations
(
opID integer primary key check(opID>0),
title varchar(100) not null,
volume integer check(volume>0)
);

create table books
(
isbn varchar(13) primary key check(length(isbn)=10 or length(isbn)=13),
publisher varchar(50) not null,
path varchar(100) not null unique,
publicationYear integer not null check(publicationYear>0),
opID integer references creations(opID) not null,
description varchar(2000)
);

create table authors
(
autID integer primary key,
firstName varchar(50) default 'Unknown',
secondName varchar(50) default 'Unknown'
);

create table creationByAuthors
(
opID integer references creations(opID) not null,
autID integer references authors(autID) not null,
primary key(opID,autID)
);

create table hierarchy
(
subgenre varchar(50) primary key,
genre varchar(50) not null
);

create table approaches
(
opID integer references creations(opID) not null,
subgenre varchar(50) references hierarchy(subgenre) not null,
primary key(opID,subgenre)
);

create table userPassword (
    username varchar(100) primary key,
    password varchar(100)
);

create table userRating (
    username varchar(100) references UserPassword(username) not null,
    ISBN varchar(13) references book(ISBN),
    rating number
);

create table userPreferences (
    username varchar(100) references UserPassword(username) not null,
    genre varchar(50),
    subgenre varchar(50),
    autID integer -- we will have to search by autID in the database
);

insert into creations values (1,'Notre-Dame de Paris',NULL);
insert into creations values (2,'Marile Sperante',1);
insert into creations values (3,'Marile Sperante',2);
insert into creations values (4,'Un yankeu la curtea regelui Arthur',NULL);
insert into creations values (5,'Ion',NULL);
insert into creations values (6,'Cartile junglei',NULL);
insert into creations values (7,'Ultimul mohican',NULL);
insert into creations values (8,'Anna Karenina',1);
insert into creations values (9,'Anna Karenina',2);
insert into creations values (10,'Ivanhoe',NULL);
insert into creations values (11,'Chemarea strabunilor',NULL);
insert into creations values (12,'Papillon',1);
insert into creations values (13,'Papillon',2);
insert into creations values (14,'Coliba unchiului Tom',NULL);
insert into creations values (15,'Gestapo',NULL);
insert into creations values (16,'Dracula',NULL);
insert into creations values (17,'Doamna Bovary',NULL);
insert into creations values (18,'Shogun',1);
insert into creations values (19,'Shogun',2);
insert into creations values (20,'Shogun',3);
insert into creations values (21, 'Femeia in rosu', NULL);
insert into creations values (22, 'Matematica M2. Manual pentru clasa a XI-a', NULL);
insert into creations values(23, 'Partea amuzanta a fizicii', NULL);

insert into authors values (1, 'Hugo', 'Victor');
insert into authors values (2,'Dickens','Charles');
insert into authors values (3,'Twain','Mark');
insert into authors values (4,'Rebreanu','Liviu');
insert into authors values (5,'Kipling','Rudyard');
insert into authors values (6,'Cooper','James-Fenimore');
insert into authors values (7,'Tolstoi','Lev');
insert into authors values (8,'Scott','Walter');
insert into authors values (9,'London','Jack');
insert into authors values (10,'Charriere','Henri');
insert into authors values (11,'Stowe','Harriet-Beecher');
insert into authors values (12,'Hassel','Sven');
insert into authors values (13,'Stoker','Bram');
insert into authors values (14,'Flaubert','Gustave');
insert into authors values (15,'Clavell','James');
insert into authors values (16,'Babeti','Adriana');
insert into authors values (17,'Mihaies','Mircea');
insert into authors values (18,'Nedelciu','Mircea');
insert into authors values (19,'Burtea','Marius');
insert into authors values (20,'Burtea','Georgeta');
insert into authors values(21, 'Michael', 'Crabtree');

insert into creationByAuthors values(1,1);
insert into creationByAuthors values(2,2);
insert into creationByAuthors values(3,2);
insert into creationByAuthors values(4,3);
insert into creationByAuthors values(5,4);
insert into creationByAuthors values(6,5);
insert into creationByAuthors values(7,6);
insert into creationByAuthors values(8,7);
insert into creationByAuthors values(9,7);
insert into creationByAuthors values(10,8);
insert into creationByAuthors values(11,9);
insert into creationByAuthors values(12,10);
insert into creationByAuthors values(13,10);
insert into creationByAuthors values(14,11);
insert into creationByAuthors values(15,12);
insert into creationByAuthors values(16,13);
insert into creationByAuthors values(17,14);
insert into creationByAuthors values(18,15);
insert into creationByAuthors values(19,15);
insert into creationByAuthors values(20,15);
insert into creationByAuthors values(21,16);
insert into creationByAuthors values(21,17);
insert into creationByAuthors values(21,18);
insert into creationByAuthors values(22,19);
insert into creationByAuthors values(22,20);
insert into creationByAuthors values(23, 21);

insert into hierarchy values('Romantic','Fictiune');
insert into hierarchy values('Drama','Fictiune');
insert into hierarchy values('Razboi','Fictiune');
insert into hierarchy values('SF','Fictiune');
insert into hierarchy values('Istorie','Fictiune');
insert into hierarchy values('Familie','Fictiune');

insert into hierarchy values('Matematica','Nonfictiune');
insert into hierarchy values('Informatica','Nonfictiune');
insert into hierarchy values('Fizica','Nonfictiune');


insert into approaches values(1,'Romantic');
insert into approaches values(1,'Drama');
insert into approaches values(2,'Drama');
insert into approaches values(3,'Drama');
insert into approaches values(4,'Istorie');
insert into approaches values(5,'Familie');
insert into approaches values(5,'Romantic');
insert into approaches values(6,'Familie');
insert into approaches values(7,'Istorie');
insert into approaches values(8,'Drama');
insert into approaches values(9,'Familie');
insert into approaches values(10,'Istorie');
insert into approaches values(11,'Familie');
insert into approaches values(12,'Drama');
insert into approaches values(13,'Drama');
insert into approaches values(14,'Familie');
insert into approaches values(15,'Istorie');
insert into approaches values(16,'Drama');
insert into approaches values(17,'Romantic');
insert into approaches values(17,'Familie');
insert into approaches values(18,'Istorie');
insert into approaches values(19,'Istorie');
insert into approaches values(20,'Istorie');
insert into approaches values(21,'Drama');
insert into approaches values(22,'Matematica');
insert into approaches values(23, 'Fizica');


insert into books values('9783161484100','Adevarul','/home/cip/Programs/RC/Proiect/1.txt',2008,1, '“Notre Dame de Paris” este povestea unei tinere prostituate, Esmeralda, care-şi exercită meseria în faţa catedralei Notre Dame. Încălcând jurămintele sfinte, arhidiaconul se îndrăgosteşte de ea şi încearcă să o seducă. Este salvată de clopotarul catedralei, diformul Quasimodo, este urmărită de poliţie şi condamnată la moarte.

Acelaşi Quasimodo o răpeşte din mâinile călăului şi o ascunde în biserică. Trădată de toată lumea, tânăra este, în final, executată, în momentul în care reuşise să-şi găsească mama.

Posteritatea “Cocoşatului de la Notre Dame”

Povestea simplă şi emoţionantă a cunoscut o posteritate impresionantă. Sub titlul “Esmeralda” sau “Cocoşatul de la Notre Dame”, a fost ecranizată de nenumărate ori, de la filmul mut din 1905, la musicalul “Notre Dame de Paris” din 1999. Actori celebri au dat viaţă personajelor principale. Gina Lollobrigida, Lesley-Anne Down, Aka Noa au fost Esmeralda, iar în rolul lui Quasimodo s-au perindat Anthony Quinn sau Anthony Hopkins.

Povestea lui Hugo a fost repovestită în cele două opere “Esmeralda”, pe muzica compusă de Dargomyzhsky, respectiv Arthur Goring Thomas, în filmul Disney adaptat în manieră “Gothic”, în musicalul rock prezentat în 1998 la Seattle pe muzica lui C. Rainez Lewis.

Potrivit datelor din Guinness Book of Records, un alt musical, canadiano-francez, din acelaşi an inspirat de „Notre Dame de Paris”, pe muzica lui Riccardo Cocciante, a fost cel mai popular exemplar al genului din toate timpurile.
');
insert into books values('9783161484101','Adevarul','/home/cip/Programs/RC/Proiect/2.txt',2008,2, 'Considerat a fi una dintre cele mai complexe lucrări ale lui Charles Dickens, romanul “Marile Speranţe”, publicat iniţial ca foileton în revista culturală “All the Year Round”, este o scriere presărată cu subtile note autobiografice, axată pe experienţele prin care trece orfanul Pip, din copilărie la maturitate.

Evoluţia protagonistului este marcată de două personaje stranii şi misterioase. Este vorba despre evadatul Abel Magwitch, care îi va schimba destinul, şi despre domnişoara Havisham, care, în urma unei decepţii, adoptase un comportament ostil faţă de bărbaţi şi în casa căreia cunoaşte primii fiori ai iubirii, nutrind o dragoste inocentă faţă de fiica adoptivă a acesteia. Romanul este plin de un suspans prin care autorul urmăreşte să menţină viu interesul cititorului. Capitolele au mereu ceva imprevizibil în ele, iar deznodământul este surprinzător. Cititorul descoperă că cele trei personaje din jurul lui Pip sunt verigi sudate în acelaşi lanţ al destinului.');

insert into books values('9783161484102','Adevarul','/home/cip/Programs/RC/Proiect/3.txt',2008,3, 'Considerat a fi una dintre cele mai complexe lucrări ale lui Charles Dickens, romanul “Marile Speranţe”, publicat iniţial ca foileton în revista culturală “All the Year Round”, este o scriere presărată cu subtile note autobiografice, axată pe experienţele prin care trece orfanul Pip, din copilărie la maturitate.

Evoluţia protagonistului este marcată de două personaje stranii şi misterioase. Este vorba despre evadatul Abel Magwitch, care îi va schimba destinul, şi despre domnişoara Havisham, care, în urma unei decepţii, adoptase un comportament ostil faţă de bărbaţi şi în casa căreia cunoaşte primii fiori ai iubirii, nutrind o dragoste inocentă faţă de fiica adoptivă a acesteia. Romanul este plin de un suspans prin care autorul urmăreşte să menţină viu interesul cititorului. Capitolele au mereu ceva imprevizibil în ele, iar deznodământul este surprinzător. Cititorul descoperă că cele trei personaje din jurul lui Pip sunt verigi sudate în acelaşi lanţ al destinului.');
insert into books values('9783161484103','Adevarul','/home/cip/Programs/RC/Proiect/4.txt',2008,4,'În anul de graţie 1835, pe planeta Pământ au avut loc cel puţin două evenimente memorabile. Primul - reapariţia Cometei Halley, ce-şi trage anevoie coada pe bolta cerească o dată cam la 76 de ani; al doilea - venirea pe lume a unuia dintre fondatorii romanului american modern, adică a lui Mark Twain. Nu ştim dacă onor criticii au luat în calcul, la atribuirea calificativului onorant, şi acest roman-plăsmuire, în fapt o spumoasă parodie la adresa legendelor despre cavalerii Mesei Rotunde şi a bisericii. Ce ştim e faptul că, la senectute, Twain a făcut către naţiunea americană următorul anunţ:

“Am sosit cu Cometa Halley în 1835. Va apărea din nou la anul, şi mă aştept să mă duc cu ea. (...) Atotputernicul a spus, fără îndoială: «Aici sunt ciudaţii ăştia doi, fără rost; au sosit împreună, trebuie să plece împreună».” Şi Twain, predicţie tulburător de exactă, a luat urma cometei în 1910, cam în aceeaşi perioadă când arătarea reapărea.');
insert into books values('9783161484104','Adevarul','/home/cip/Programs/RC/Proiect/5.txt',2008,5, 'Romanul “Ion” ne introduce în viaţa de dinainte de război a ţăranilor şi intelectualilor ardeleni. Ion al Glanetaşului, fiu harnic al unor părinţi săraci, pune ochii pe Ana, fata bogătaşului Vasile Baciu. Din dorinţa de ascensiune rapidă şi având patima pământului în sânge, Ion o necinsteşte pe Ana, ca să-l oblige pe Baciu să i-o dea cu tot cu avere. Scopul este atins, Ion devenind stăpân al pământurilor Anei. Fericit, sărută pământul câştigat. Ajuns aici, eroul proaspăt îmbogăţit e stăpânit de iubirea pentru frumoasa Florica, simpatie veche, măritată la rândul ei. Ademenită şi nechibzuită, Florica răspunde pasiunii nebune a lui Ion, riscând tot ceea ce clădise alături de soţul ei. Bătută şi chinuită de bărbat, dar şi de tată, Ana decide să-şi curme viaţa, luând cu ea şi copilul nenăscut. Liber de constrângeri, Ion dă frâu liber pasiunii nebuneşti pe care o are pentru Florica, urmând a fi surprins de soţul acesteia şi ucis în faptul nopţii.

În 1920 romanul “Ion” i-a adus lui Liviu Rebreanu consacrarea în literatura română, precum şi premiul Academiei Române.');
insert into books values('9783161484105','Adevarul','/home/cip/Programs/RC/Proiect/6.txt',2008,6, 'Atras de omul simplu, cu personalitatea nealterată de ispitele lumii civilizate, Rudyard Kipling a susţinut pe tot parcursul vieţii "misiunea civilizatoare" a Angliei în lumea haotică a Indiei. Petrecând primii ani ai copilăriei în India, acea lume fabuloasă, cu o natură luxuriantă, imaginaţia i-a fost în permanenţă aprinsă de tradiţiile şi poveştile localnicilor.

Volumul “Cărţile Junglei” reuneşte Prima şi a Doua Carte a Junglei, care evocă într-o manieră alertă, aproape cinematografică, viaţa unui băiat crescut de lupi. Având ca prieteni fiarele sălbatice ale junglei indiene, în alte situaţii înfricoşătoare, Mowgli este nevoit să poarte o luptă pe care nu şi-o doreşte, împotriva tigrului Shere Khan. Revenit printre oameni, va fi pus în faţa altei dileme: se va adapta el oare la viaţa socială a celor care îl cred fiul demult dispărut? Citiţi această poveste emoţionantă în traducerea lui Mihnea Gheorghiu şi descoperiţi deliciile literaturii de aventuri pentru toate vârstele!');

insert into books values('9783161484106','Adevarul','/home/cip/Programs/RC/Proiect/7.txt',2008,7, 'James Fenimore Cooper a fost unul din cei mai populari şi mai prolifici autori ai secolului al XIX-lea. Ultimul Mohican este considerat a fi capodopera sa. Romanul a fost foarte bine primit la vremea când a fost scris. Şi acum, după mai bine de 180 de ani de la prima publicare, cititori din lumea întreagă urmăresc cu sufletul la gură aventurile lui Ochi-de-Şoim, Chingachgook şi ale fiului său, Uncas. Cooper a dat numele personajului Uncas după o persoană reală, indian din tribul Mohegan. Faptul că autorul a folosit acest nume pentru un personaj al tribului Mohican (sau Mahican), produce şi acum confuzii printre cei ce cunosc istoria acestor triburi.

Povestea relatează evenimente din timpul războiului dintre francezi şi indieni, în anul 1757, când Franţa şi Marea Britanie s-au luptat pentru a deţine controlul coloniilor din America şi Canada. Titlul cărţii a fost inspirat de o declaraţie a lui Tamanend, una din căpeteniile celui mai important clan care a fondat Philadelphia:

“Am trăit să-l văd pe ultimul luptător al Mohicanilor, una din cele mai înţelepte naţii.”');
insert into books values('9783161484107','Adevarul','/home/cip/Programs/RC/Proiect/8.txt',2008,8, 'Deşi Lev Tolstoi a început să lucreze la romanul Anna Karenina în 1873, ideea intrigii i-a venit în 1870, când s-a decis să scrie despre o femeie căsătorită, din înalta societate, care "s-a dat singură pierzării". Intenţia a fost de a zugrăvi personajul în aşa fel încât să fie demnă de milă, fără a fi însă vinovată. Merită menţionat faptul că scriitorul a luat ca model pentru eroina sa pe fiica poetului Maria Alexandrovna Hartung, imortalizându-i trăsăturile expresive în imaginea Annei. Încordarea şi pasiunea cu care a lucrat la acest roman au fost atât de intense, încât romanul a fost terminat, în ciornă, în 50 de zile.

Însă pe tot parcursul procesului creativ, Tolstoi a fost extrem de nemulţumit de ceea ce rezulta în urma zilelor pline de lucru. Atât intriga, cât şi personajele, au suferit modificări numeroase. Levin, de pildă, nu exista în versiunile iniţiale ale romanului, apărând ulterior sub numele de Ordînţev sau Neradov. Vronski se numea Balaşev, Gaghin, Udaşev, iar Alabin, soţul lui Dolly, cu prenumele alternând: Stepan şi Mihail.');
insert into books values('9783161484108','Adevarul','/home/cip/Programs/RC/Proiect/9.txt',2009,9, 'Lev Tolstoi considera că Anna Karenina a fost primul său roman în adevăratul sens al cuvântului, acesta reprezentând un adevărat apogeu al romanului realist.

Eroina, iniţial Tatiana Stavrovici, viitoarea Anna Karenina, a pornit de la un personaj prin excelenţă negativ, nu prea frumoasă dar atrăgătoare, uşor vulgară, superficială şi cochetă. Anna urma să devina adulteră mai degrabă din perversitate decât din pasiune În ce măsură chipul definitiv diferă de această primă “apariţie” ne-o arată chiar paginile romanului, începând cu portretul fizic, Tolstoi oferind impresia că a mobilizat întreaga măiestrie pentru a sugera, încă de la intrare, cât de frumoasă e eroina lui în sensul cel mai deplin al cuvântului. Frumuseţea Annei devine un leit-motiv al întregii acţiuni. Totdată Anna e şi un om superior, cu calităţi care o singularizează parcă în raport cu grupul personajelor care o înconjoară.');
insert into books values('9783161484109','Adevarul','/home/cip/Programs/RC/Proiect/10.txt',2009,10, 'Romanul relatează despre perioada de după a Treia Cruciadă, în 1194, când majoritatea cruciaţilor se întorc în Europa, iar Regele Richard este capturat de Ducele de Saxonia. Legendarul Robin de Locksley, cunoscut cititorilor ca Robin Hood şi ceata lui de oameni veseli sunt doar câteva dintre personajele acestui roman. Caracterizarea lui Robin Hood făcută de Walter Scott a creionat figura veselă a vestitului nelegiuit pe care-l ştim cu toţii.

Cartea a fost scrisă şi publicată într-o perioadă de lupte strânse pentru emanciparea evreilor din Anglia, cu critici aspre la adresa nedreptăţilor împotriva lor. Ivanhoe îl însoţeşte pe Richard în Cruciade, unde se presupune că a avut un rol foarte important, mai ales în asediul de la Acre.');
insert into books values('9783161484118','Adevarul','/home/cip/Programs/RC/Proiect/11.txt',2009,11, 'La vremea când rebelul Jack se hotărâse să devină scriitor (refuzând postul bine remunerat de poştaş), literatura americană abundă de subiecte “care să fie potrivite pentru ochii şi urechile domnişoarelor”, tabu-uri fiind ateismul, socialismul şi... picioarele femeilor. Or, realismul scrierilor autodidactului încrâncenat, de mic vânzător de ziare, “pirat de stridii”, copil al căilor ferate, cerşetor şi vagabond ajuns la închisoare, nu putea fi pe placul tinerelor de familie bună amatoare de lectură.

Chiar şi aşa, în câteva dintre scrierile lui Jack London iese totuşi în evidenţă sensibilitatea cu care el a creat personaje din lumea necuvântătoarelor: uriaşul câine Buck, din “Chemarea străbunilor”, parcurge un drum invers celui al evoluţiei speciei sale: ajuns din însorita Californie printre gheţurile Alaskăi, după ce stăpânul său este ucis se alătură lupilor, dominând sălbatica haită.');
insert into books values('9783161484122','Adevarul','/home/cip/Programs/RC/Proiect/12.txt',2009,12, 'Ca prim roman al lui Henri Charrière, publicat în 1970, “Papillon” a avut un succes enorm şi imediat. Volumul descrie experienţele personale pe care Charrière le-a trăit ca prizonier în Guiana Franceză. Acuzat de uciderea unui proxenet, condamnat şi încarcerat în 1932, Charrière a avut numeroase încercări de a evada, pentru a putea repara nedreptatea ce i se făcuse. A susţinut încă de la început că este nevinovat, fiind nevoit să-şi părăsească soţia însărcinată şi fiica şi să ia calea închisorii. După câteva încercări de evadare eşuate, Papillon este în sfârşit eliberat şi se stabileşte în Venezuela. Titlul cărţii este dat de porecla pe care Henri Charrière a primit-o în închisoare, după tatuajul în formă de fluture pe care îl avea pe piept.

Veridicitatea poveştii a fost de multe ori contestată de unii critici, dar el a susţinut întotdeauna că tot ce a scris este adevărat, cu mici scăpări din cauza golurilor de memorie. În 2005 la Paris a apărut un anume Charles Brunier, care a pretins că este adevăratul Papillon. Mulţi au fost cei care au dat crezare acestuia, cu atât mai mult cu cât în anii prezentaţi în roman, Brunier fusese coleg cu Charrière.');
insert into books values('9783161484132','Adevarul','/home/cip/Programs/RC/Proiect/13.txt',2009,13,'Ca prim roman al lui Henri Charrière, publicat în 1970, “Papillon” a avut un succes enorm şi imediat. Volumul descrie experienţele personale pe care Charrière le-a trăit ca prizonier în Guiana Franceză. Acuzat de uciderea unui proxenet, condamnat şi încarcerat în 1932, Charrière a avut numeroase încercări de a evada, pentru a putea repara nedreptatea ce i se făcuse. A susţinut încă de la început că este nevinovat, fiind nevoit să-şi părăsească soţia însărcinată şi fiica şi să ia calea închisorii. După câteva încercări de evadare eşuate, Papillon este în sfârşit eliberat şi se stabileşte în Venezuela. Titlul cărţii este dat de porecla pe care Henri Charrière a primit-o în închisoare, după tatuajul în formă de fluture pe care îl avea pe piept.

Veridicitatea poveştii a fost de multe ori contestată de unii critici, dar el a susţinut întotdeauna că tot ce a scris este adevărat, cu mici scăpări din cauza golurilor de memorie. În 2005 la Paris a apărut un anume Charles Brunier, care a pretins că este adevăratul Papillon. Mulţi au fost cei care au dat crezare acestuia, cu atât mai mult cu cât în anii prezentaţi în roman, Brunier fusese coleg cu Charrière.');
insert into books values('9783161484142','Adevarul','/home/cip/Programs/RC/Proiect/14.txt',2009,14,'Istoricii au afirmat că romanul Coliba unchiului Tom a avut un impact atât de mare asupra publicului, încât a condus la declanşarea Războiului Civil, în urma căruia sclavia a fost abolită. Se spune că, întâlnind-o pe Harriet Beecher Stowe, la începutul războiului, Abraham Lincoln a afirmat: “Deci aceasta este mica doamnă care, prin cartea ei, a pornit un război atât de mare”. Coliba unchiului Tom este romanul prin care cunoscuta scriitoare americană a declamat cruzimea şi ororile sclavagiste.

Ajuns în imposibilitatea de a-şi întreţine una din ferme, Arthur Shelby, un agricultor din Kentucky, are o singură soluţie pentru a scăpa de datorii: să-şi vândă doi sclavi. Unul dintre aceştia este unchiul Tom, un bărbat respectabil cu soţie şi copii, celălalt este Harry, fiul Elizei, menajera soţiei agricultorului. Neputând îndura gândul de a fi despărţită de fiul ei pentru tot restul vieţii, Eliza fuge împreună cu acesta, asumându-şi toate riscurile pe care le implică acest gest disperat şi, asemenea celorlalte personaje, trece prin experienţe cutremurătoare.');
insert into books values('9783161484152','Adevarul','/home/cip/Programs/RC/Proiect/15.txt',2009,15,'În acest volum, ritmul scrierii se apropie de cel cinematografic, autorul creând totodată o reuşită alternanţă a situaţiilor. Astfel, de la disciplina penitenciară, în fapt o terifiantă batjocură la adresa condamnaţilor, acţiunea este translatată spre o “paranghelie cazonă”. De la arestarea unei bătrânici, care va fi condamnată la moarte pentru ofensă la adresa Führerului, se ajunge la întâlnirea a doi afacerişti în uniformă - Porta şi trompetul-cavalerist Rudolf Kleber, care, suprem argument, i-a cântat lui Hitler. Trocul cu ţigări cu opiu contra poze porno e savuros, portretele celor două personaje fiind creionate cu un indiscutabil umor (amintind de Hasek şi al său brav soldat Svejk).

Hassel reuşeşte, de asemenea, o paralelă şocantă între teroarea de tip Gestapo şi aceea, cu nimic mai prejos, practicată de NKVD-ul sovietic. Ceea ce induce cititorului credinţa că între svastică şi crucea verde a comisarilor sovietici au existat doar deosebiri de nuanţă. Altfel spus, cam aceeaşi Mărie, cu altă pălărie.

Alte două personaje care provoacă cel puţin repulsie sunt gardianul şef al închisorii Altona, Stabsfeldwebelul Stalschmidt şi Standartenführerul SD Paul Bielert.');
insert into books values('9783161484162','Adevarul','/home/cip/Programs/RC/Proiect/16.txt',2009,16, 'Scrisă in 1897 de irlandezul Bram Stoker, povestea contelui Dracula a fost inclusă in foarte multe genuri literare. Deşi roman epistolar, unii au spus că face parte din literatura horror, alţii l-au clasat drept roman gotic.

Deşi prima menţiune despre vampiri a fost făcută cu mult înainte de apariţia acestui roman, volumul "Dracula" a contribuit la răspândirea termenului in toate cercurile. Au urmat multe ecranizări şi puneri în scenă, atât recent după apariţia cărţii, cât şi în secolul XXI.

Romanul începe cu călătoria lui Jonathan Harker, un avocat debutant, către castelul Contelui Dracula, de la graniţa Transilvaniei cu Moldova. În curând, tânărul işi va da seama că este prizonier în castelul gazdei sale. Din acel moment începe să observe laturile îngrijorătoare ale vieţii lui Dracula...');
insert into books values('9783161484172','Adevarul','/home/cip/Programs/RC/Proiect/17.txt',2009,17,'Gustave Flaubert, cel care, peste ani, a stârnit admiraţia lui Mario Varga Llosa, a abordat subiectul "femeia, eterna poveste" din perspectiva condiţiei acesteia în societatea franceză a secolului al XlX-lea.

Cronicari şi exegeţi ai scriitorului au susţinut şi suţin că Madame Bovary este romanul ce dă întreaga măsură a talentului lui Flaubert, el prezentând destinul nefericit al Emmei printr-o alternanţă de planuri - real, imaginar - ce suscită de la prima până la ultima pagină interesul cititorilor.

Pasiunile nebuneşti, luxul visat, viaţa paralelă, negarea rolului de soţie şi mamă, toate conduc destinul nefericitei tinere către sinucidere, către izbăvitoarea doză de arsenic...

Să mai amintim că titlul iniţial era Madame Bovary, moravuri de provincie şi că după apariţia cărţii scriitorului i s-a intentat proces pentru ultraj adus moralei publice şi religiei. Flaubert a fost achitat, iar operele lui au fost şi sunt citite în întreaga lume.');
insert into books values('9783181484118','Polirom','/home/cip/Programs/RC/Proiect/18.txt',2001,11,'La vremea când rebelul Jack se hotărâse să devină scriitor (refuzând postul bine remunerat de poştaş), literatura americană abundă de subiecte “care să fie potrivite pentru ochii şi urechile domnişoarelor”, tabu-uri fiind ateismul, socialismul şi... picioarele femeilor. Or, realismul scrierilor autodidactului încrâncenat, de mic vânzător de ziare, “pirat de stridii”, copil al căilor ferate, cerşetor şi vagabond ajuns la închisoare, nu putea fi pe placul tinerelor de familie bună amatoare de lectură.

Chiar şi aşa, în câteva dintre scrierile lui Jack London iese totuşi în evidenţă sensibilitatea cu care el a creat personaje din lumea necuvântătoarelor: uriaşul câine Buck, din “Chemarea străbunilor”, parcurge un drum invers celui al evoluţiei speciei sale: ajuns din însorita Californie printre gheţurile Alaskăi, după ce stăpânul său este ucis se alătură lupilor, dominând sălbatica haită.');
insert into books values('9789734608815','Polirom','/home/cip/Programs/RC/Proiect/19.txt',2008,21, '„Femeia in rosu este considerata o carte de virf a valului celui mai recent de literatura romana, un roman postmodern. Un etalon al romanului romanesc.” (Ion Bogdan Lefter)

„Femeia in rosu, demonstratie teoretica si practica de virtuozitate, este o etalare de forta exact in momentul in care viata se pregateste sa-si ia revansa asupra artificialului, adica atunci cind memoriile, autofictiunea, jurnalele si dezlantuirile amoroase dadeau sa ecluzeze manierata, stilata, rafinata «doamna» optzecista.” (Simona Sora)');

insert into books values('9789731908052','PriorBooks','/home/cip/Programs/RC/Proiect/20.txt',2011,22, 'Manual pentru clasa a XI-a. Este recomandat elevilor conștiincioși care vor să se pregătească temeinic pentru examenul de bacalaureat.');

insert into books values('9789731908053','PriorBooks','/home/cip/Programs/RC/Proiect/24.txt',1995,23, 'În această capodoperă, ni se explică cum fizica poate deveni mult mai interesantă.');
create view PreviewInformation as
select title, volume, firstName, secondName, isbn, publisher, publicationYear, genre, subgenre, rating, description
from books natural join creations natural join authors natural join creationByAuthors natural join hierarchy natural join approaches
natural left outer join (select isbn, avg(rating) as "rating" from userRating group by isbn);


insert into userPassword values('Cip', 'helloworld');
insert into userPassword values('Alex', 'celmaismecher');
insert into userPassword values('Super', 'noneedforapassword');
