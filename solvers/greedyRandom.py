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
##monster
#* x
#* y
#* hp
#* exp
#* id
#* gold

##hero
#* base_speed
#* base_power
#* base_range
#* level_speed_coeff
#* level_power_coeff
#* level_range_coeff


hero["exp"] = 0
hero["level"] = 0
hero["speed"] = hero["base_speed"]
hero["power"] = hero["base_power"]
hero["range"] = hero["base_range"]

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


def get_almost_closest_monster():
    closest_monster = None
    monsters.sort(key=lambda x: (x["x"] - pos[0])**2 + (x["y"] - pos[1])**2)
    if len(monsters) == 0:
        return None
    
    return monsters[random.randint(0, min(20, len(monsters)-1))]

closest_monster = get_almost_closest_monster()

def exp_cutoff():
    return 1000 + hero["level"]*(hero["level"]+1)*50

def update_hero():

    while hero["exp"] >= (exp_cut:=exp_cutoff()):
        hero["level"] += 1
        hero["exp"]-= exp_cut
    
    #⌊𝑏𝑎𝑠𝑒_𝑠𝑝𝑒𝑒𝑑 ⋅ (1 + 𝐿 ⋅ 𝑙𝑒𝑣𝑒𝑙_𝑠𝑝𝑒𝑒𝑑_𝑐𝑜𝑒𝑓𝑓/100 )⌋

    hero["speed"] = int(hero["base_speed"] * (1 + hero["level"] * hero["level_speed_coeff"]/100))
    hero["power"] = int(hero["base_power"] * (1 + hero["level"] * hero["level_power_coeff"]/100))
    hero["range"] = int(hero["base_range"] * (1 + hero["level"] * hero["level_range_coeff"]/100))

for z in range(turns):
    #print(pos)
    if closest_monster is None:
        break

    target_dist_sqr = (closest_monster["x"] - pos[0])**2 + (closest_monster["y"] - pos[1])**2

    if target_dist_sqr <= hero["range"]**2:
        sol["moves"].append({
            "type": "attack",
            "target_id": closest_monster["id"],
            "comment": f"Hero level: {hero['level']}, Hero exp: {hero['exp']}, Hero speed: {hero['speed']}, Hero power: {hero['power']}, Hero range: {hero['range']}, Monster Dist: {target_dist_sqr}, target hp: {closest_monster['hp']}, target id: {closest_monster['id']}"
        })

        closest_monster["hp"] -= hero["power"]

        if closest_monster["hp"] <= 0:
            monsters.remove(closest_monster)
            hero["exp"] += closest_monster["exp"]
            closest_monster = get_almost_closest_monster()
            update_hero()
    
    else:


        target_x, target_y = max_step(pos, [closest_monster["x"], closest_monster["y"]], hero["speed"])


        sol["moves"].append({
            "type": "move",
            "target_x": target_x,
            "target_y": target_y,
            "comment": f"Hero level: {hero['level']}, Hero exp: {hero['exp']}, Hero speed: {hero['speed']}, Hero power: {hero['power']}, Hero range: {hero['range']}"
        })
        pos = [target_x, target_y]


print(json.dumps(sol))



