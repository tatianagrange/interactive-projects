#La Plante qui Tweet
---

## 1. Introduction
Arduino est une petite carte électronique avec des entrées et des sorties sur lesquelles il est facile de venir se brancher.

Cette carte a été créer en Italie pour des élèves qui n'avaient que peu de connaissance en électronique.

Elle est open-source et open-hardware. Cela signifie que tous les plans de la cartes, ainsi que le logiciel, se trouvent en ligne et accessible à tous. On trouve donc beaucoup d'autres versions d'arduino. Comme les funduino, par exemple.

Ce projet utilise une carte Arduino Yun. Il s'agit d'une carte qui possède, en plus des entrées et sorties habituelles, un petit OS Linux embarqué, ainsi que du wifi. On peut donc facilement communiquer avec internet et accéder à la carte sans fil.

## 2. Twitter et temboo
Pour que la plante puisse *tweeter*, il faut autoriser cela sur son compte twitter. Il faut ensuite accéder à l'API de twitter, qui permet de nombreuses choses.

Temboo est une Library qui permet d'accéder à de nombreuses API. C'est pourquoi nous allons utiliser ce service afin de connecter la carte.

Le tuto pour autoriser l'accès à Twitter depuis temboo se trouve [ici](https://www.temboo.com/library/Library/Twitter/Tweets/StatusesUpdate/).

## 3. Connecter la carte

La carte Arduino Yun peut-être entièrement controllée depuis un ordinateur, sans fils. Pour cela, il faut indiquer le wifi sur lequel elle doit se connecter.

Afin de paramètrer cela, la carte Arduino Yun génère un wifi de type `Arduino Yun-XXXXXXXXX`. Si ce n'est pas le cas, appuler une dizaine de secondes sur le bouton `WLAN RST` et attendez quelques secondes.

**Attention**, vous si vous êtes plusieurs dans la salle, assurez-vous qu'il s'agit bien de votre carte.

Suite à cela, vous pouvez vous connecter sur l'interface locale que génère la Arduino Yun. Il faut se connecter à [http://arduino.local](http://arduino.local). Le mot de passe est `Arduino`.

Il faut ensuite paramètrer le réseau auquel on se connecte. La carte redémarre et on connecte l'ordinateur sur le même réseau que celui renseigné à l'Arduino.

## 4. Connecter la carte au logiciel

Un environnement de développement existe pour travailler sur Arduino. Il est téléchargeable [ici](http://arduino.cc/en/Main/Software). Il faut prendre la version compatible avec l'Arduino Yun, actuellement la *1.5.8*.

Une fois le logiciel installé, il faut renseigner le type de carte `Outils > Type de Carte > Arduino Yùn`. Puis il faut renseigner le port `Arduino at 192.168.X.X`

**Attention**, si vous êtes plusieurs à utiliser la carte au même endroit, vérifiez que vous vous connectez à la bonne carte.

## 5. Envoyer un premier test sur la carte

Il faut aller chercher un code d'exemple dans la bibliothèque d'Arduino. Le Scketch (code) basique s'appel `Blink` et consiste à faire clignoter une petite LED sur la carte. Une led rouge.

Le code est coposé de deux fonctions. `setup`, qui est appelé une fois au démarage du programme. Et `loop`, qui est appelée en boucle jusqu'à ce que l'on éteigne la carte.

Il faut lancer le scketch sur la carte en appuyant sur la flèche. La led clignote? Parfait!

## 6. Lancer le code de github
Le code récupéré sur github est un peu plus compliqué que le que celui du `Blink`

###Le fichier .h
Dans le projet, il y a un fichier header. Un `TembooAccount.h.default`. Il faut le copier dans un fichier `TembooAccount.h` et y placer les `#define` générés sur Temboo.

Il sert à bien séparer les identifiants de Temboo.

###Le fichier .ino
Il s'agit du projet en lui même. Il est commenté (dans un anglais très approximatif) et divisé en plusieurs partis.

####Les Variables
Elles vont nous permettre de définirs les pins utilisées sur la carte Arduino, ainsi que les valeurs seuils. Elle vont aussi permettre de retenir des valeurs de ce que l'on calcul dans la boucle d'un tour sur l'autre, comme le temps qui s'est écoulé depuis le dernier tweet, ou le nombre de tweets.

####Main Code
Même si elles sont plus longues, il s'agit des mêmes fonctions que dans `Blink`: `setup` et `loop`.

Dans `loop`, on va suivre l'algorithme suivant:

	x <- Humidité de la plante
	SI x < seuil d'humidité ET il est temp de tweeter
		incrémentation du nombre de tweet
		connexion à temboo
		ajout de la phrase à tweeter
		tentative de tweet
		SI le tweet est réussi
			Faire clignoter la led doucement
		SINON
			écrire l'erreur
			Faire clignoter beaucoup et vite
		FIN SI
	SINON
		remettre le nombre de tweet à zéro
	FIN SI
	
####Functions
Il s'agit des fonctions pratique pour le code. On y retrouve les fonctions suivante:
* connectToTemboo: elle sert à se ronnecter à temboo
* getTimeStamp: retourne le temps, en secondes, depuis 1970. Le Timestamp.
* getDate: retourne la date qui s'affiche dans le tweet
* timeIsChek: vérifie le temps entre le dernier tweet et l'heure actuelle. Selon le nombre de tweets demandés dans la journée, il va retourner vrai quand cela sera nécessaire
* makeTweet: construit le tweet en fonction du nombre de tweets déjà envoyés.








