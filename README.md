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

## JsonToTxt input format

    Line 01: base_speed
    Line 02: base_power
    Line 03: base_range
    Line 04: level_speed_coef
    Line 05: level_power_coef
    Line 06: level_range_coef
    Line 07: start_x
    Line 08: start_y
    Line 09: width
    Line 10: height
    Line 11: num_turns
    Line 12: m (number of monsters)
    Line 13-13+m-1: Monster in the form (x,y,hp,gold,exp)


## Links
* https://codeforces.com/blog/entry/129585
* https://codeweekend.dev/
* https://codeweekend.dev/#/api
* https://discord.gg/M6pG5zp3DF

## Strategies
* Simulated Annealing: write a function that finds the optimal solution given the order in which the monsters have to be attacked, then run SA on that order
* Monte Carlo Tree Search: run a search up to some depth, then evaluate that position by running greedy solution
* ML magic: build an ML model that can evaluate how good a current state is
