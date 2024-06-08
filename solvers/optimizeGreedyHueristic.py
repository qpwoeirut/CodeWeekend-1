from scipy.optimize import minimize, basinhopping, dual_annealing
import numpy as np
import random
import json
import sys

#tc = json.loads(sys.stdin.read())

for randomvar in range(12, 14, 1):
    path = f"heropath_inputs_day1/{format(randomvar,'03')}.json"
    def f(params):
        tc = json.loads(open(path).read())
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


        def get_almost_closest_monster():
            monsters.sort(key=lambda x: (((x["x"] - pos[0])**2 + (x["y"] - pos[1])**2)) - x["gold"]/(x["hp"]/hero["power"])*params[0] - (x["exp"]*params[1] if hero["level"] < 3 else  0))
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
                # sol["moves"].append({
                #     "type": "attack",
                #     "target_id": closest_monster["id"],
                #     "comment": f"Hero level: {hero['level']}, Hero exp: {hero['exp']}, Hero speed: {hero['speed']}, Hero power: {hero['power']}, Hero range: {hero['range']}, Monster Dist: {target_dist_sqr}, target hp: {closest_monster['hp']}, target id: {closest_monster['id']}"
                # })

                closest_monster["hp"] -= hero["power"]

                if closest_monster["hp"] <= 0:
                    
                    hero["exp"] += closest_monster["exp"]
                    gold += closest_monster["gold"]
                    monsters.remove(closest_monster)
                    closest_monster = get_almost_closest_monster()
                    update_hero()
            
            else:

                target_x, target_y = max_step(pos, [closest_monster["x"], closest_monster["y"]], hero["speed"])

                
                # sol["moves"].append({
                #     "type": "move",
                #     "target_x": target_x,
                #     "target_y": target_y,
                #     "comment": f"Hero level: {hero['level']}, Hero exp: {hero['exp']}, Hero speed: {hero['speed']}, Hero power: {hero['power']}, Hero range: {hero['range']}"
                # })
                pos = [target_x, target_y]

        return -gold


    initial_guess = np.array([50.0,100.0,10])

    methods = ['nelder-mead', 'Powell', 'CG', 'BFGS', 'L-BFGS-B', 'TNC', 'COBYLA', 'SLSQP', 'trust-constr', 'dogleg', 'trust-ncg', 'trust-exact', 'trust-krylov']

    #res = minimize(f, initial_guess, method='nelder-mead', options={'xatol': 1e-8, 'disp': True}, bounds=[(0,1000),(0,1000),(0,100)])
    #res = minimize(f, initial_guess, method='Powell', options={'disp': True}, bounds=[(0,1000),(0,1000),(0,100)])
    #res = minimize(f, initial_guess, method='CG', options={'disp': True})

    """
    for method in methods:
        res = minimize(f, initial_guess, method=method, options={'disp': True}, bounds=[(0,1000),(0,1000),(0,100)])
        print(res)
        print(res.x)
        print(-res.fun)
    """

    #res = minimize(f, initial_guess, method='COBYLA', options={'disp': True}, bounds=[(0,1000),(0,1000),(0,100)])

    #print(res)
    #print(res.x)
    #print(-res.fun)


    #print(json.dumps(sol))

    # try dual annealing
    res = dual_annealing(f, bounds=[(0,100),(0,100)], maxiter=1000)
    #print(res)
    print("tc = ",randomvar)
    print("params = ",list(res.x))
    print(-res.fun)
