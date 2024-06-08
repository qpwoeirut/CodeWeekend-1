# CodeWeekend-1
Team CPCHEC's code for Code Weekend #1

# TODO STUFFFFFFFFFFFF
- Create Scoring Function for TCS if not provided one
- Calculate Theoretical Max Values for TCS then store somewhere to see progress
- Brain storm IDEAS and useful UTILS that can be used
- Learn Optimization Techniques
- Use API to get current stats of our team like scores (can be used for next step)
- Auto submit (when found better sol) + auto grade  
- Use API to graph the progress of the competition 
- ~~Create team name animation with the API~~ ![trust](https://cdn.discordapp.com/emojis/1204640543053709454.webp?size=48&quality=lossless&name=trust)

## Links
* https://codeforces.com/blog/entry/129585
* https://codeweekend.dev/
* https://codeweekend.dev/#/api
* https://discord.gg/M6pG5zp3DF

## Strategies
* Simulated Annealing: write a function that finds the optimal solution given the order in which the monsters have to be attacked, then run SA on that order
* Monte Carlo Tree Search: run a search up to some depth, then evaluate that position by running greedy solution
* ML magic: build an ML model that can evaluate how good a current state is
