# Projet Capteur Graphite
## Context
Ce projet a été réalisé dans le cadre du cours « Du capteur au banc de test », durant le deuxième semestre de l’année universitaire 2025–2026, au sein de la formation de 4ᵉ année en Génie Physique. Il s’appuie sur l’article scientifique « Pencil Drawn Strain Gauges and Chemiresistors on Paper » de Cheng-Wei Lin, Zhibo Zhao, Jaemyung Kim et Jiaxing Huang, qui étudie les propriétés du graphite et démontre son potentiel en tant que matériau sensible pouvant être utilisé comme capteur de déformation.

L’idée principale de ce projet est de concevoir un capteur de déformation low-tech, inspiré de cette publication, en utilisant uniquement une feuille de papier et une couche de graphite d'un crayon déposée par simple tracé.

L’objectif est d’évaluer les performances, la fiabilité de ce capteur à faible coût et de vérifier s’il peut être commercialisé.
## Sommaire
- [Context](#context)
- [Sommaire](#sommaire)
- [Principe de fonctionnement du capteur en graphite](#principe-de-fonctionnement-du-capteur-en-graphite)
- [Réalisation du projet](#réalisation-du-projet)
  - [Matériaux nécessaire](#matériaux-nécessaire)
  - [Simulation électronique sur LTSpice](#simulation-électronique-sur-ltspice)
  - [Conception PCB avec Kidcad](#conception-pcb-avec-kidcad)
  - [Réalisation du Shield](#réalisation-du-shield)
  - [Code Arduino](#code-arduino)
  - [Conception application sur MIT App Inventor](#conception-application-sur-mit-app-inventor)
  - [Bancs de test](#bancs-de-test)
    - [Sensibilité](#sensibilité)
    - [Répétabilité](#répétabilité)
- [Datasheet](#datasheet)
- [Conclusion](#conclusion)

## Principe de fonctionnement du capteur en graphite
<div align="center">
  <img src="Images/Schema_graphite_physics.png" width="400">
  <br/>
  <b>Figure 1: Principe de fonctionnement du capteur graphite en traction et en compression</b>
</div>
  <br/>

La mine de crayon est déposée sur le papier de manière mécanique. Elle pénètre dans la structure poreuse du papier et forme une piste conductrice composée de particules de graphite mélangées à un liant isolant, comme l’argile. Ces particules créent un réseau conducteur à l’intérieur des fibres du papier. Quand le papier est soumis à une contrainte mécanique, ce réseau change : sous traction, les contacts entre les particules diminuent, tandis que sous compression, ils augmentent. Ces modifications provoquent alors des variations mesurables de la résistance électrique.

## Réalisation du projet
Afin d’atteindre les objectifs fixés, le travail a été divisé en deux tâches principales : 

1. la mesure de la résistance du capteur en graphite et son affichage via une interface

2. la réalisation de bancs de test pour évaluer l’efficacité de ce capteur
<div align="center">
  <img src="Images/Montage_de_mesure.jpg" width="400">
  <br/>
  <b>Figure 2: Montage expérimental de mesure du capteur graphite</b>
</div>

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
Dans le cas réel, la résistance du capteur en graphite est très élevée, ce qui conduit à un courant très faible, de l’ordre de 100 à 150 nA. Dans ces conditions, une mesure directe de la résistance à l’aide d’un multimètre n’est pas optimale.
Nous avons donc choisi de mesurer ce faible courant à l’aide d’un microcontrôleur. L’Arduino Uno a été retenu car il est open source, simple à utiliser, et bénéficie d’une large communauté, ce qui nous rend plus facile pour une première approche des microcontrôleurs.

En raison du courant très faible fourni par le capteur, nous avons utilisé un amplificateur transimpédance pour transformer ce courant en une tension mesurable par l’Arduino. Le gain du circuit peut être modifié simplement en changeant la valeur de la résistance R2.

Pour limiter le bruit, en particulier le bruit du réseau électrique à 50 Hz, trois filtres passe‑bas ont été ajoutés. Ces filtres permettent aussi de s’assurer que le signal en sortie du circuit est compatible avec la fréquence d’acquisition de l’Arduino.

<div align="center">
  <img src="Images/Circuit_analogique_schematic.png" width="700">
  <br/>
  <b>Figure 3: Circuit d’amplification et de filtrage du signal</b>
</div>  
  <br/>
<table>
  <tr>
    <td align="center">
      <img src="Images/Capteur_graphite_schematic.png" height="450" width="100%">
      <br/>
      <b>Figure 4: Modèle électrique du capteur graphite</b>
    </td>
    <td align="center">
      <img src="Images/Courant_graphite_LTSpice.png" height="450" width="100%">
      <br/>
     <b>Figure 5: Courant du capteur graphite simulé sous LTSpice (sans bruit)</b>
    </td>
  </tr>
</table>



Pour la simulation, le capteur graphite réel est modélisé dans LTSpice par un circuit électronique dans le figure , qui produit une sortie de courant d’environ 150 nA conformément à la valeur réalité. Le sous‑ensemble C3, R5 et le générateur V2 permet de simuler les bruits provenant de l’environnement.

<table>
  <tr>
    <td align="center">
      <img src="Images/Tension_entree_LTSpice.png" height="450" width="100%" alt="Hình 1">
      <br/>
      <b>Figure 6: Tension d’entrée du circuit d'amplificateur simulée sous LTSpice</b>
    </td>
    <td align="center">
      <img src="Images/Tension_sortie_LTSpice.png" height="450" width="100%" alt="Hình 2">
      <br/>
     <b>Figure 7: Tension de sortie du circuit (V_ADC) simulée sous LTSpice</b>
    </td>
  </tr>
</table>

Les simulations montrent une tension d’entrée du circuit d’environ 18 mV avant filtrage et amplification. En sortie, le signal est correctement amplifié et filtré, comme illustré sur la figure correspondante. Le niveau d’amplification obtenu dépend directement de la valeur choisie pour R2.

### Conception PCB avec Kidcad
<div align="center">
  <img src="Images/Schematic_KiCad.png" width="700">
  <br/>
  <b>Figure 8: Schéma électronique global du système sous KiCad</b>
</div>  
  <br/>
<table>
  <tr>
    <td align="center">
      <img src="Images/PCB_Kicad.png" width="100%">
      <br/>
      <b>Figure 9: Routage du PCB conçu sous KiCad </b>
    </td>
    <td align="center">
      <img src="Images/PCB_3D_Kicad.png" width="100%">
      <br/>
     <b>Figure 10: Vue 3D du PCB conçu sous KiCad</b>
    </td>9
  </tr>
</table>


### Réalisation du Shield
<table width="100%">
  <tr>
    <td width="50%" align="center">
      <img src="Images/Shield_face_arriere.jpg" height="250">
    </td>
    <td width="50%" align="center">
      <img src="Images/Shield_face_avant.jpg" height="250">
    </td>
  </tr>
</table>


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
  <img src="Images/Menu_principal_oled.jpg" width="400">
  <br/>
  <b>Figure 13: Menu principal affiché sur l’écran OLED</b>
</div> 
  <br/>
<div align="center">
  <img src="Images/Gain_menu_oled.jpg" width="400">
  <br/>
  <b>Figure 14: Menu de réglage du gain affiché sur l’écran OLED</b>
</div>
  <br/>
  Il suffit d’appuyer sur l’encodeur pour revenir au menu principal
  
  <br/>
<table>
  <tr>
    <td align="center">
      <img src="Images/Servomoteur_menu1.jpg" width="100%">
      <br/>
      <b>Figure 15: Menu de sélection de l’angle du servomoteur affiché sur l’écran OLED</b>
    </td>
    <td align="center">
      <img src="Images/Servomoteur_menu2.jpg" width="100%">
      <br/>
     <b>Figure 16: Menu de sélection de l’angle du servomoteur affiché sur l’écran OLED</b>
    </td>
    <td align="center">
      <img src="Images/Servomoteur_menu3.jpg" width="100%">
      <br/>
      <b>Figure 17: Menu d'application de l’angle du servomoteur affiché sur l’écran OLED</b>
    </td>
  </tr>
</table>


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
  <img src="Images/Menu_principal_app.jpg" width="300">
  <br/>
  <b>Figure 18: Interface principale de l’application Android</b>
</div>  
  <br/>

<p align="center">
  <img src="Images/Signal_real_time_menu.gif" width="300"/>
  <br/>
  <b>Figure 19: Démonstration de la variation de résistance du capteur en graphite sous déformation sur l'application Android</b>
</p>

<div align="center">
  <img src="Images/Mesuring_resistance_menu.png" width="600">
  <br/>
  <b>Figure 20: Interface de mesure de la résistance dans l’application Android</b>
</div>


### Bancs de test
Afin d’étudier les caractéristiques et d’évaluer les performances du capteur en graphite, deux bancs de test ont été mis en place.
Ces bancs permettent d’analyser en particulier la sensibilité et la rentabilité du capteur, en comparaison avec un capteur flex commercial.
#### Sensibilité
Pour mesurer la sensibilité des capteurs, un modèle mécanique imprimé en 3D a été utilisé.

<div align="center">
  <img src="Images/Test_bench_sensibilite.jpg" width="400">
  <br/>
  <b>Figure 21: Protocole de banc de test de sensibilité</b>
</div>

Ce modèle est constitué de plusieurs arcs de cercle de rayons différents, chaque rayon correspondant à un niveau de déformation différent appliqué au capteur.

Le capteur est successivement placé sur chaque arc, ce qui permet d’imposer une déformation contrôlée, aussi bien en traction (tension) qu’en compression.
La même procédure est appliquée au capteur graphite et au capteur flex, afin de permettre une comparaison directe.
Pour le capteur graphite, les mesures ont été réalisées avec différents types de graphite : HB, 2B, 4B et 6B.
Les résultats obtenus sont présentés sur les figures suivantes, montrant l’évolution du rapport ΔR/R0 en fonction de la déformation, en traction et en compression.
<div align="center">
  <img src="Images/Tension_graph.png" width="600">
  <br/>
  <b>Figure 22: Variation du rapport ΔR/R0​ en fonction de la déformation en tension pour différents types de graphite et le capteur flex</b>
</div>
  <br/>


<div align="center">
  <img src="Images/Compression_graph.png" width="600">
  <br/>
  <b>Figure 23: Variation du rapport ΔR/R0​ en fonction de la déformation en compression pour différents types de graphite et le capteur flex</b>
</div>

##### Résultat
<div align="center">
  <img src="Images/Sensibilite_graphite.png" width="600">
  <br/>
  <b>Figure 24: Résultats de sensibilité des capteurs graphite (HB, 2B, 4B, 6B)</b>
  <img src="Images/Sensibilite_flex.png" width="600">
  <br/>
  <b>Figure 25: Résultats de sensibilité du capteur flex en traction et en compression</b>
</div>  

Les mesures montrent une relation globalement linéaire entre la variation de résistance ΔR/R0​ et la déformation appliquée, aussi bien en traction qu’en compression. Cela indique que les capteurs réagissent de manière cohérente aux déformations imposées.

Pour le capteur graphite, le graphite de type 2B présente la sensibilité la plus élevée, aussi bien en traction qu’en compression, comparé aux graphites HB, 4B et 6B. En comparaison avec le capteur flex :

* en traction, la sensibilité du capteur flex est supérieure à celle du graphite 2B ;
* en compression, la sensibilité du capteur flex est très faible, ce qui est cohérent avec le fait que ce type de capteur est principalement conçu pour fonctionner en traction, comme spécifié par le fabricant.
#### Répétabilité
Pour vérifier la répétabilité du capteur, nous avons mis en placeun banc de test simple.
Comme montré sur les figures, un servomoteur est relié au capteur à l’aide d’un petit bâton en bois. L’ensemble du montage est fixé avec de la colle afin de limiter les déplacements parasites pendant les mesures.
Le servomoteur applique successivement un angle de 0° puis de 120° au capteur. Ce cycle est répété cinq fois de suite.
Ce test est réalisé avec un capteur graphite 2B, car ce type de graphite a montré la meilleure sensibilité lors des essais précédents. Le même protocole est également appliqué au capteur flex afin de pouvoir comparer les résultats.
<div align="center">
  <img src="Images/Test_bench_servomoteur.jpg" width="500">
  <br/>
  <b>Figure 26: Protocole de banc de test de répétabilité avec servomoteur</b>
</div>

##### Résultat

<table width="100%">
  <tr>
    <td width="50%" align="center">
      <img src="Images/Repetabilite_graphite_graph1.png" height="250">
    </td>
    <td width="50%" align="center">
      <img src="Images/Repetabilite_flex_graph1.png" height="250">
    </td>
  </tr>
</table>

<table>
  <tr>
    <td align="center">
      <img src="Images/Repetabilite_graphite_graph2.png" height="300" width="100%">
      <br/>
      <b>Figure 29: Répétabilité du capteur graphite 2B – valeur moyenne et écart‑type</b>
    </td>
    <td align="center">
      <img src="Images/Repetabilite_flex_graph2.png" height="300" width="100%">
      <br/>
     <b>Figure 30: Répétabilité du capteur flex – valeur moyenne et écart‑type</b>
    </td>
  </tr>
</table>

Les figures 1 et 2 montrent la valeur de la résistance mesurée pour chaque angle, avec une courbe correspondant à chaque répétition du test.
Les figures 3 et 4 présentent la valeur moyenne de la résistance pour chaque angle. Les barres d’erreur représentent l’écart‑type calculé à partir des différentes mesures.
Pour le capteur flex, la répétabilité est très bonne : les courbes sont presque superposées et l’écart‑type est très faible, au point d’être difficilement visible sur le graphique.
Pour le capteur graphite 2B, on observe un léger décalage entre les différentes répétitions. La répétabilité est donc un peu moins bonne que celle du capteur flex. Toutefois, l’écart entre les mesures reste faible, de l’ordre de 0,5 % de la valeur moyenne.

### Datasheet
### Conclusion
### Remarque/Difficulte
### Contact