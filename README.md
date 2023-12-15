# ants-aichallenge-bot
Bot expérimental pour le jeu http://ants.aichallenge.org/index.php

# Bots gagnants
 - #1 : [xathis](https://github.com/xathis/AI-Challenge-2011-bot) : [post mortem](https://web.archive.org/web/20160624203328/http://xathis.com/posts/ai-challenge-2011-ants.html)
 - #2 : [GreenTea](https://sourceforge.net/projects/ants2011/) : [post mortem](https://web.archive.org/web/20120111120814/http://www.trevoroakes.com/blog/2011/12/23/greenteas-2nd-place-entry-postmortem-translated-by-hand/)
 - #4 : runevision : [post mortem](https://blog.runevision.com/2011/12/ai-challenge-my-bot-explained-part-i.html)
 - #8 : FlagCapper : [post mortem](https://web.archive.org/web/20131101052116/http://flagcapper.com/?c2) + [video](http://www.youtube.com/watch?v=vsbda_hL2Ro)
 - #10 : Memetix : [post mortem](www.blackmoor.org.uk/AI.htm)

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
## ADVANCED_GAME :
Condition :
- \>= 18 fourmis
- < 43 fourmis

Répartition :
- 3+ fourmis gardes                          [GUARDING]
- 5* fourmis en formation                    [ROAMING]

Comportement :
- Toujours en formation                      [FORMING] (formation x)
- Attaque d'ennemis                          [ATTACKING]
- Récolte de bouffe                          [GATHERING]

Sécurité :
- Une équipe reste collée à la fourmilière

Objectif :
> Récolte de bouffe 100% sécuritaire

## WINNING :
Condition :
- \>= 43 fourmis

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