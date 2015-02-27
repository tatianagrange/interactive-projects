#NFC & Relay

Le but du projet est de créé un système sur Arduino capable d'activer, grâce à un relais, un mécanisme électrique qui nécessite un accès autorisé.

Le NFC permettra aux utilisateurs du lieu d'accéder ou non au mécanisme, grâce aux identifiants stocké sur la carte SD.

##Les librairies
Voici les librairies utilisées dans le projet:

* [NDEF](https://github.com/don/NDEF) - Pour NFCAdapter
* [PN532](https://github.com/Seeed-Studio/PN532) - Pour le module NFC

##test - lcdandnfc

Ce projet Arduino contient un premier test.

* Si je montre une carte NFC:
	* Affichage de l'id et d'un décompte (70 secondes)
	* A la fin du décompte, un buzzer *bip* pendant une seconde, puis propose de repasser sa carte.
* Sinon, il ne se passe rien.

##project

Il s'agit du projet à proprement parlé. En plus du fichier de test, il inclue le log sur la carte SD des cartes qui ont *badgées*.