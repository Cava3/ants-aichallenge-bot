# ants-aichallenge-bot
Bot expérimental pour le jeu http://ants.aichallenge.org/index.php

# 4 phases de jeu :

## DISCOVERING :
Condition :
- < 5 fourmis

Répartition :

- 1+ fourmis exploratrices                  [EXPLORING] 

Comportement :
- Fuite systématique                        [FLEEING]   
- Ramassage de bouffe                       [GATHERING] 

Sécurité :
- Restent proches de la fourmilière

Objectif :
> Créer assez de fourmis pour avoir une garde sécurisée

---
## SLOW_PLAYING :
Condition :
- \>= 5 fourmis
- < 9 fourmis

Répartition :
- 3 fourmis gardes                          [GUARDING]  
- 2+ fourmis exploratrices                  [EXPLORING] 

Comportement :
- Ramassage de bouffe                       [GATHERING] 
- Fuite systématique                        [FLEEING]

Sécurité :
- Une équipe reste collée à la fourmilière

Objectif :
> Créer assez de fourmis pour avoir une armée en défendant la fourmilière

---
## MAIN_GAME :
Condition :
- \>= 9 fourmis
- < 18 fourmis

Répartition :
- 3+ fourmis gardes                          [GUARDING] 
- 3* fourmis en formation                    [ROAMING]  



Comportement :
- Ramassage de bouffe                        [GATHERING]
- Mise en formation pour combattre           [FORMING] (formation |)

Sécurité :
- Une équipe reste collée à la fourmilière 

Objectif :
> Former des groupes de 3 fourmis pour qu'elles puissent gagner les combats

---
## WINNING :
Condition :
- \>= 18 fourmis

Répartition :
- 3+ fourmis gardes                          [GUARDING] 
- 5* fourmis en formation                    [ROAMING]    

Comportement :
- Toujours en formation                      [FORMING] (formation x)
- Attaque d'ennemis                          [ATTACKING]
- Destruction de fourmilières ennemies       [DESTROYING]

Objectif :
> Exterminer  

---  

*Les répartitions (+) acceuillent les nouveaux arrivants*
*Lorsqu'il y a assez de recrues pour une équipe (\*) on en crée une*