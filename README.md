# Projet Capteur Graphite
## Context
Ce projet a été réalisé dans le cadre du cours « Du capteur au banc de test », durant le deuxième semestre de l’année universitaire 2025–2026, au sein de la formation de 4ᵉ année en Génie Physique. Il s’appuie sur l’article scientifique « Pencil Drawn Strain Gauges and Chemiresistors on Paper » de Cheng-Wei Lin, Zhibo Zhao, Jaemyung Kim et Jiaxing Huang, qui étudie les propriétés du graphite et démontre son potentiel en tant que matériau sensible pouvant être utilisé comme capteur de déformation.

L’idée principale de ce projet est de concevoir un capteur de déformation low-tech, inspiré de cette publication, en utilisant uniquement une feuille de papier et une couche de graphite d'un crayon déposée par simple tracé.

L’objectif est d’évaluer les performances, la fiabilité de ce capteur à faible coût et de vérifier s’il peut être commercialisé.
## Sommaire

## Réalisation du projet
Afin d’atteindre les objectifs fixés, le travail a été divisé en deux tâches principales : 

1. la mesure de la résistance du capteur en graphite et son affichage via une interface

2. la réalisation de bancs de test pour évaluer l’efficacité de ce capteur
### Matériaux nécessaire
Pour réaliser le shield, nous avons besoin du matériel suivant :

* 1 carte Arduino UNO Rev 3
* 1 module Bluetooth (HC-05)
* 1 encodeur rotatif (KY-040)
* 1 écran OLED (Joy-it SBC-OLED01)
* 1 capteur de flexion commercial (Spectra Symbol Flex Sensors 005 21)
* 1 capteur en graphite fabriqués de la mine d'un crayon de papier
* 1 amplificateur (LTC1050)
* 1 potentiomètre numérique (MCP41010)
* 1 résistance 1kΩ, 1 résistance 10kΩ, 2 résistances 100kΩ et 1 résistance 27kΩ
* 3 condensateurs 100nF, 1 condensateur 1µF
* 2 supports IC pour le potentiomètre et l'amplificateur
* Pin headers (male et female)

Pour les bancs de test :
* 1 modèle 3D à diamètres variables
* 1 servomoteur (Parallax standard servo)

### Simulation électronique sur LTSpice
### Conception PCB avec Kidcad

### Réalisation du Shield
### Code Arduino
Nous avons développé un programme sous Arduino IDE.  Dans le code, nous avons ajouté les librairies Adafruit_SSD1306 pour piloter l'écran OLED et SoftwareSerial pour le Bluetooth HC-05. Pour rendre le code plus clair et mieux organisé, le programme Arduino est divisé en plusieurs fichiers :

* Le fichier "menu_OLED" est utilisé pour contrôler le potentiomètre numérique donc régler le gain.
* Le fichier "digipot" gère l’affichage sur l’écran OLED et la lecture de l’encodeur rotatif.
* Le fichier principal (main) "capteur_projet" s’occupe du module Bluetooth, du servomoteur, et contient les fonctions setup et loop.

Ce programme permet : 
* de sélectionner le niveau d’amplification du signal
* de contrôler un servomoteur
* de communiquer avec une application Android grâce à un module Bluetooth intégré au shield, permettant ainsi l’échange de données entre le capteur et l’interface mobile

Ces deux premières fonctionnalités sont pilotées via un écran OLED pour l’affichage et un encodeur rotatif utilisé pour la navigation et la sélection dans les menus.

<div align="center">
  <img src="Images/998466956897953028.jpg" width="300">
  <br/>
  <b>Figure 1: Menu principal</b>

<div align="center">
  <img src="Images/2832030047224382400.jpg" width="300">
  <br/>
  <b>Figure 1: Gain menu</b>
  <br/>
  Il suffit d’appuyer sur l’encodeur pour revenir au menu principal

<table>
  <tr>
    <td align="center">
      <img src="Images/343478864478811174.jpg" width="300" alt="Hình 1">
      <br/>
      <b>Figure </b>
    </td>
    <td align="center">
      <img src="Images/343478864478811174 (1).jpg" width="300" alt="Hình 2">
      <br/>
     <b>Figure </b>
    </td>
    <td align="center">
      <img src="Images/1051818459947776580.jpg" width="300" alt="Hình 3">
      <br/>
      <b>Figure </b>
    </td>
  </tr>
</table>
<div style="text-align: left;">

Lorsque l’utilisateur entre dans le menu du servomoteur, l’écran de la figure 1 s’affiche.
En appuyant sur l’encodeur, il est possible de sélectionner un angle : le texte « Choose an angle » passe en majuscules (figure 2).
Un nouvel appui sur l’encodeur permet d’appliquer l’angle sélectionné au servomoteur, et le mot « APPLY » apparaît en majuscules (figure 3).
La sortie du menu servomoteur s’effectue en cliquant deux fois de suite sur l’encodeur rotatif.

Concernant la fonctionnalité du code pour communiquer avec l'application Android, en fonction des commandes reçues depuis l’application via module Bluetooth, le programme Arduino peut renvoyer à l’application :
* soit en continu (chaque 100 miliseconds) la valeur de la résistance du capteur en graphite ainsi que la tension en sortie du circuit d’amplification (tension ADC) 
* soit de manière ponctuelle la résistance du capteur en graphite
* ou de manière ponctuelle la résistance du flex sensor

Pour obtenir la valeur de la résistance du capteur, le code Arduino commence par convertir la lecture analogique (comprise entre 0 et 1023) en une tension comprise entre 0 et 5 V.
La résistance du capteur en graphite et du capteur flex est ensuite calculée à partir de cette tension en utilisant la formule suivante :

* $R_{\text{graphite}} = \left(1 + \frac{R_3}{R_2}\right) \cdot R_1 \cdot \left(\frac{V_{\text{CC}}}{V_{\text{ADCgraphite}}}\right) - R_1 - R_5$

* $R_{\text{flex}} = R_{\text{Division}} \cdot \frac{V_{\text{flex}}}{V_{\text{CC}} - V_{\text{ADCflex}}}$
### Conception application sur MIT App Inventor
Pour assurer la communication avec l’utilisateur et l’affichage des données, une application Android a été développée à l’aide de MIT App Inventor.

Cette application communique avec l’Arduino via une connexion Bluetooth et intègre deux fonctionnalités principales :

* Affichage en temps réel de la variation de la résistance du capteur graphite lorsqu’une déformation est appliquée. La tension ADC est également affichée afin de permettre à l’utilisateur d’ajuster le gain lorsque le signal est saturé ou trop faible. (Figure)
* Mesure ponctuelle de la résistance du capteur sélectionné lors de l’appui sur le bouton Measure. L’utilisateur peut saisir la valeur de référence du capteur à l’état non déformé, puis l’application calcule automatiquement le rapport ΔR/R0. (Figure)

<div align="center">
  <img src="Images/7d024398-bd45-47b4-8095-33f9a691c197.jpg" width="300">
  <br/>
  <b>Figure 1: Menu principal</b>


<video src="Images/download.mp4" controls width="600">
    Votre navigateur ne prend pas en charge la vidéo.  
</video>
  <b>Figure 1: Menu principal</b>

<div align="center">
  <img src="Images/mesuring_resistance.png" width="300">
  <br/>
  <b>Figure 1: Menu principal</b>
<div style="text-align: left;">


### Banc de test
### Datasheet
### Conclusion
### Remarque/Difficulte
### Contact