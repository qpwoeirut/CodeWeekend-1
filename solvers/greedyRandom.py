import json
import sys
import random


tc = json.loads(sys.stdin.read())

#print(tc)

width = tc["width"]
height = tc["height"]
turns = tc["num_turns"]
pos = [tc["start_x"], tc["start_y"]]

hero = tc["hero"]
monsters = tc["monsters"]
##hero
#* base_speed
#* base_power
#* base_range
#* level_speed_coeff
#* level_power_coeff
#* level_range_coeff

sol = {
    "moves": [

    ]
}

for i,monster in enumerate(monsters):
    monster["id"] = i

#print(monsters)

def max_step(start, end, speed):
    minx = min(start[0], end[0])
    maxx = max(start[0], end[0])
    miny = min(start[1], end[1])
    maxy = max(start[1], end[1])

    best_point = list(start)
    max_dist_sqr = 0

    for i in range(minx,maxx+1):
        for j in range(miny,maxy+1):
            if (i-start[0])**2 + (j-start[1])**2 <= speed**2 and (i-start[0])**2 + (j-start[1])**2 > max_dist_sqr:
                max_dist_sqr = (i-start[0])**2 + (j-start[1])**2
                best_point = [i,j]

    return best_point


def get_random_monster():
    if len(monsters) == 0:
        return None
    return random.choice(monsters)

cur_monster = get_random_monster()
    

for z in range(turns):
    #print(pos)
    if cur_monster is None:
        break

    dist_sqr = (cur_monster["x"] - pos[0])**2 + (cur_monster["y"] - pos[1])**2

    if dist_sqr <= hero["base_range"]**2:
        sol["moves"].append({
            "type": "attack",
            "target_id": cur_monster["id"]
        })

        cur_monster["hp"] -= hero["base_power"]

        if cur_monster["hp"] <= 0:
            monsters.remove(cur_monster)
            cur_monster = get_random_monster()
    
    else:
        #move towards monster integer coord with dist base_speed


        #hero["base_speed"]

        target_x, target_y = max_step(pos, [cur_monster["x"], cur_monster["y"]], hero["base_speed"])


        sol["moves"].append({
            "type": "move",
            "target_x": target_x,
            "target_y": target_y,
        })
        pos = [target_x, target_y]


print(json.dumps(sol))




