import json
import sys
import random
import numpy as np


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

    ],
    "gold" : 0
}

for i,monster in enumerate(monsters):
    monster["id"] = i

#print(monsters)

def max_step(start, end, speed):
    start = np.array(start)
    end = np.array(end)
    
    # Calculate the direction vector from start to end
    direction = end - start
    
    # Calculate the distance between start and end
    distance = np.linalg.norm(direction)
    
    # If the end point is within the speed distance, return the end point
    if distance <= speed:
        return [int(end[0]), int(end[1])]
    
    # Normalize the direction vector
    unit_direction = direction / distance
    
    # Scale the unit direction vector by speed
    max_step_vector = unit_direction * speed
    
    # Calculate the new point
    best_point = start + max_step_vector
    
    # Adjust the point to the nearest integer coordinates
    best_point_int = np.round(best_point).astype(int)
    
    # Ensure the integer point is within the speed distance and on the safe side
    while np.linalg.norm(best_point_int - start) > speed:
        best_point_int -= np.sign(best_point_int - start)
    
    return [int(best_point_int[0]), int(best_point_int[1])]

#01 params =  [28.93515348, 65.260799 ,  96.28266209]
#02 params =  [13.42005061,  0.31609578 ,12.81335043]
#03 params =  [56.52765456, 83.96080018 , 68.78751141]
#04 params =  [56.08611732, 49.20014679, 69.57645266]
#05 params =  [1.2781016559350036, 198.32996143507876, 76.03397659349497]
#06 params =  [0.8285738248042946, 247.79635936568388, 41.68645548631321]
#07 params =  [1.7014010120937684, 294.8799026328652, 98.48668192850204]
#08 params =  [25.326578487811247, 158.95622361715436, 89.19960404165397]
#09 params =  [36.337888927633514, 216.35748827584587, 83.31417506489291]
#10 params =  [4.653443565936783, 75.63917417215214, 97.68259341669756]
#11 params =  [7.478849495171602, 120.31675441016114, 74.4512089108095]
#12 params =  [92.91905893295234, 213.1839101496007, 69.56938296416774]
#13 params =  [95.34410132118501, 285.9204654603118, 0.9535893984138966]
#14 params =  [12.478035697790773, 129.2201044065477, 30.094522934221857]
#15 params =  [24.32844029699936, 103.8785619360857, 68.21017872784756]
#!16 params =  [42.74092933630203, 154.1774561907834, 3.873163170715628]
params =  [9.568355629773052, 1.8202205656990031, 20.682475072451524]
#23 params =  [1.0851067781295427, 17.574197330940592, 3.0513616778388837]
#24 params =  [0.8860507570522458, 57.000785807680586, 3.749960505077425]
#25 params =  [55.733522884622744, 0.6843198217341069, 0.7886124350516184]
def get_almost_closest_monster():
    monsters.sort(key=lambda x: ((x["x"] - pos[0])**2 + (x["y"] - pos[1])**2) - x["gold"]*params[0] - (x["exp"]*params[1] if hero["level"] < 3 else  0) + x["hp"]*params[2])
    if len(monsters) == 0:
        return None
    
    return monsters[0]

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

gold = 0

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
            hero["exp"] += closest_monster["exp"]
            gold += closest_monster["gold"]
            monsters.remove(closest_monster)
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

sol["gold"] = gold
print(json.dumps(sol))




