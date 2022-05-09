# TP - Ligne de champs éléctriques

## **Introduction**

Un champ éléctrique est le champ vectoriel créée par des particules éléctriquement chargées. Quand plusieurs charges sont à proximité de l'autres ces lignes de champs éléctriques diffère prenant en compte prenant en compte si la charges est chargée positivement ou négativement. La visualisation des lignes de champs éléctrique permet d'apercevoir directement ce qui se passe entre plusieurs charges qui pour nous la plupart du temps est invisible à l'oeil nu. Et que le comportement des lignes de champs éléctriques est différent dépendant du nombre de charges et du signe de chaque charge(positif ou négatif). De ce fait nous allons donc voir comment ai-je réaliser la visualisation de ces lignes de champs éléctriques, commençant tout d'abord par la réalisation des dessin de chaque charge. Puis la réalisation du dessin de chaque ligne de champ à partir d'un ensemble de particules dans notre univers. Puis comment j'ai assembler ces deux procédé pour avoir le tout sur l'écran(charges + lignes de champs éléctriques).

## **Réalisation du dessin des charges éléctriques**

Tout d'abord, chaque charge éléctrique sera représenté d'un cercle avec un plus ou moins à l'intérieur dépendant du signe de la charge:  

![Dessin charges à partir de la librairie gfx](./Images/Dessin_charges.png)

<!-- <p align="center">
    <img alt="Dessin des charges" src="./Images/Dessin_charges.png">
</p> -->

  
Pour représenter les droites du signe plus et moins à l'écran, j'ai utilisé l'algorithme de tracé de droites de Bresenham. Cela permet de tracer chaque pixel de la droite entre deux points pour que la droite soit plus précise sans saut incohérent de pixels comme ci-dessous: 

![Tracé de Bresenham(Extrait de wikipédia)](./Images/Trace_Bresenham.png)
<!-- <p align="center">
    <img alt="Trace de Bresenham" src="./Images/Trace_Bresenham.png" title="Extrait de Wikipedia">
</p> -->

Pour le cercle autour de chaque charge, j'ai utilisé l'algorithme de tracé de cercle d'Andres. Cet algorithme permet de tracer des cercles en image matricielle, et donc de pouvoir dessiner un cercle sans laisser de trous. Le principe est de paver entièrement le plan par des cercles concentriques.

En assemblant ces deux différentes méthodes, j'ai pu dessiner le cercle autour de la charge avec un rayon donné puis dessiner le plus et le moins dépendant du signe de la charge. Le signe "plus" se dessine dans mon cas, en traçant une droite verticalement partant de la position de la charge plus ou moins le rayon de charge/2 :  
**Point 1**: x=pos(Charge).x+(rayon(Charge)/2) // y=pos(charge).y  
**Point 2**: x=pos(Charge).x-(rayon(Charge)/2) // y=pos(charge).y


Et de tracer également une droite horizontalement:  
**Point 1**: x=pos(Charge).x // y=pos(Charge).y+(rayon(Charge)/2)  
**Point 2**: x=pos(Charge).x // pos(Charge).y-(rayon(Charge)/2)

Pour le signe "moins", il suffit juste de reprendre le tracé horizontal.  

## **Réalisation de chaque ligne de champ**  


