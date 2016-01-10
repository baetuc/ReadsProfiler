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
rating number check(rating>=0 and rating<=10),
numberOfRates integer default 0 check(numberOfRates>=0),
opID integer references creations(opID) not null
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


insert into books values('9783161484100','Adevarul','/home/cip/Programs/RC/Proiect/1.txt',2008,9.8,0,1);
insert into books values('9783161484101','Adevarul','/home/cip/Programs/RC/Proiect/2.txt',2008,8,0,2);
insert into books values('9783161484102','Adevarul','/home/cip/Programs/RC/Proiect/3.txt',2008,7.3,0,3);
insert into books values('9783161484103','Adevarul','/home/cip/Programs/RC/Proiect/4.txt',2008,8.7,0,4);
insert into books values('9783161484104','Adevarul','/home/cip/Programs/RC/Proiect/5.txt',2008,5,0,5);
insert into books values('9783161484105','Adevarul','/home/cip/Programs/RC/Proiect/6.txt',2008,7,0,6);
insert into books values('9783161484106','Adevarul','/home/cip/Programs/RC/Proiect/7.txt',2008,9,0,7);
insert into books values('9783161484107','Adevarul','/home/cip/Programs/RC/Proiect/8.txt',2008,10,0,8);
insert into books values('9783161484108','Adevarul','/home/cip/Programs/RC/Proiect/9.txt',2009,9.8,0,9);
insert into books values('9783161484109','Adevarul','/home/cip/Programs/RC/Proiect/10.txt',2009,8.7,0,10);
insert into books values('9783161484118','Adevarul','/home/cip/Programs/RC/Proiect/11.txt',2009,7,0,11);
insert into books values('9783161484122','Adevarul','/home/cip/Programs/RC/Proiect/12.txt',2009,9.5,0,12);
insert into books values('9783161484132','Adevarul','/home/cip/Programs/RC/Proiect/13.txt',2009,9.1,0,13);
insert into books values('9783161484142','Adevarul','/home/cip/Programs/RC/Proiect/14.txt',2009,5.5,0,14);
insert into books values('9783161484152','Adevarul','/home/cip/Programs/RC/Proiect/15.txt',2009,8.5,0,15);
insert into books values('9783161484162','Adevarul','/home/cip/Programs/RC/Proiect/16.txt',2009,9,0,16);
insert into books values('9783161484172','Adevarul','/home/cip/Programs/RC/Proiect/17.txt',2009,7,0,17);
insert into books values('9783181484118','Polirom','/home/cip/Programs/RC/Proiect/18.txt',2001,9,0,11);
insert into books values('9789734608815','Polirom','/home/cip/Programs/RC/Proiect/19.txt',2008,4,0,21);

insert into books values('9789731908052','PriorBooks','/home/cip/Programs/RC/Proiect/20.txt',2011,8,0,22);

create view PreviewInformation as
select title, volume, firstName, secondName, isbn, publisher, publicationYear, genre, subgenre, rating
from books natural join creations natural join authors natural join creationByAuthors natural join hierarchy natural join approaches;
