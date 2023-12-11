use std::iter::Cycle;
use std::str::Chars;
use std::collections::HashMap;

pub fn solve(instructions: &mut Cycle<Chars<'_>>, nodes_map: &HashMap<String, (String,String)>) {
    let mut current = "AAA";
    let mut step = 0;
    while !is_goal(current) {
        step += 1;
        let next_instruction = instructions.next().unwrap();
        current = match next_instruction {
            'L' => &nodes_map.get(current).unwrap().0,
            'R' => &nodes_map.get(current).unwrap().1,
            _ => panic!("illegal move"),
        };
    }
    println!("{}", step);
}


fn is_goal(node: &str) -> bool {
    node == "ZZZ"
}
