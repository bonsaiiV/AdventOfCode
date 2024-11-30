
fn _part2_bad_bruteforce(hail_storms: &Vec<Storm>) {
    let mut offset_boundry = 1;
    let mut found = false;
    while !found {
        println!("offset: {}", offset_boundry);
        let first_offset = offset_boundry;
        for second_offset in 1..offset_boundry + 2 {
            //println!("Next offset: ({}, {})", first_offset-1, second_offset);
            if _test_stone_dir(hail_storms, first_offset - 1, second_offset) {
                found = true;
            } else if _test_stone_dir(hail_storms, second_offset - 1, first_offset) {
                found = true;
            }
        }
        offset_boundry += 1;
    }
}
fn _test_stone_dir(storms: &Vec<Storm>, first_offset: u32, second_offset: u32) -> bool {
    let storm_pairs = storms.iter().flat_map(|storm2| {
        storms
            .iter()
            .filter(|&storm1| storm1.direction != storm2.direction)
            .zip(repeat(storm2))
    });
    for (storm1, storm2) in storm_pairs {
        let direction = (storm2.support + storm2.direction * (second_offset + first_offset).into()
            - (storm1.support + storm1.direction * first_offset.into()))
            / second_offset.into();
        let stone = Storm {
            support: storm1.support + (storm1.direction - direction) * first_offset.into(),
            direction: direction,
        };
        //println!("\n{:?} and\n{:?} result in Stone:\n{:?}\n", storm1, storm2, stone);
        let collisions = storms
            .iter()
            .map(|storm| {
                //println!("Testing {:?}", storm);
                let collision_time =
                    (stone.support.x - storm.support.x) / (storm.direction.x - stone.direction.x);
                //println!("Collision time is {:?}", collision_time);
                let storm_pos = storm.support + storm.direction * collision_time;
                let stone_pos = stone.support + stone.direction * collision_time;
                //println!("Collision pos is {:?} or {:?}", storm_pos, stone_pos);
                if stone_pos == storm_pos {
                    Some(storm_pos)
                } else {
                    None
                }
            })
            .collect::<Vec<_>>();
        if collisions
            .iter()
            .all(|collision_opt| collision_opt.is_some())
        {
            println!("{}", stone.support.abs());
            return true;
        }
    }
    false
}
