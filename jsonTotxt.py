import json

TCS = 50

for i in range(1, TCS+1):
    data = json.load(open(f"inputs/{format(i,'03')}.json"))
    with open(f"inputs/{format(i,'03')}.txt", "w") as f:
        #f.write("hero\n")
        for k, v in data["hero"].items():
            #f.write(f"{k} {v}\n")
            f.write(f"{v}\n")
        
        #f.write(f"start_x {data['start_x']}\n")
        f.write(f"{data['start_x']}\n")
        #f.write(f"start_y {data['start_y']}\n")
        f.write(f"{data['start_y']}\n")
        #f.write(f"width {data['width']}\n")
        f.write(f"{data['width']}\n")
        #f.write(f"height {data['height']}\n")
        f.write(f"{data['height']}\n")
        #f.write(f"num_turns {data['num_turns']}\n")
        f.write(f"{data['num_turns']}\n")
        #f.write(f"monsters {len(data['monsters'])}\n")
        f.write(f"{len(data['monsters'])}\n")
        #f.write("x y hp gold exp\n")
        for monster in data["monsters"]:
            f.write(f"{monster['x']} {monster['y']} {monster['hp']} {monster['gold']} {monster['exp']} {0 if i <= 25 else monster['range']} {0 if i <= 25 else monster['attack']}\n")

    print(f"Converted testcase {i} from JSON to TXT")
print("Done")