import json
import sys


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


def get_closest_monster(monsters, pos):
    closest_monster = None
    closest_distance = 1000000
    for monster in monsters:
        dist_sqr = (monster["x"] - pos[0])**2 + (monster["y"] - pos[1])**2
        if dist_sqr < closest_distance:
            closest_distance = dist_sqr
            closest_monster = monster
    return closest_monster, closest_distance

closest_monster, closest_distance = get_closest_monster(monsters, pos)

for z in range(turns):
    #print(pos)
    if closest_monster is None:
        break
    if closest_distance <= hero["base_range"]**2:
        sol["moves"].append({
            "type": "attack",
            "target_id": closest_monster["id"]
        })

        closest_monster["hp"] -= hero["base_power"]

        if closest_monster["hp"] <= 0:
            monsters.remove(closest_monster)
            closest_monster, closest_distance = get_closest_monster(monsters, pos)
    
    else:
        #move towards monster integer coord with dist base_speed


        #hero["base_speed"]

        target_x, target_y = max_step(pos, [closest_monster["x"], closest_monster["y"]], hero["base_speed"])


        sol["moves"].append({
            "type": "move",
            "target_x": target_x,
            "target_y": target_y,
        })
        pos = [target_x, target_y]


print(json.dumps(sol))




