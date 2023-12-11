use regex::Regex;

use std::io::{self, BufRead};
use std::fs::File;

fn main() {
    let re = Regex::new(r"([A-Z]{3}) = \(([A-Z]{3}), ([A-Z]{3})\)").unwrap();
    let input_reader = io::BufReader::new(File::open("resources/input").unwrap());
    let mut lines = input_reader.lines();
    let ref_instructions = lines.next().unwrap().unwrap();
    lines.next();
    let nodes_vec = lines.map(|line| {
        let line_str = line.unwrap();
        let (_, [node, left, right]) = re.captures(&line_str).unwrap().extract();
        (node.to_owned(),(left.to_owned(),right.to_owned()))
    }).collect::<Vec<(String,(String,String))>>();
    let nodes_map = nodes_vec.iter().map(|(node, (left,right))| (node, (get_index(left,&nodes_vec).unwrap(),get_index(right,&nodes_vec).unwrap()))).collect::<Vec<(&String,(usize,usize))>>();
    let mut current = get_index("AAA", &nodes_vec).unwrap();
    let goal = get_index("ZZZ", &nodes_vec).unwrap();
    let mut instructions = ref_instructions.chars();
    let mut step = 0;
    let mut next_instruction;
    while current != goal {
        step += 1;
        if let Some(tmp_instruction) = instructions.next(){
            next_instruction = tmp_instruction;
        }else{
            instructions = ref_instructions.chars();
            next_instruction = instructions.next().unwrap();
        }
        current = match next_instruction{
            'L' => nodes_map.get(current).unwrap().1.0,
            'R' => nodes_map.get(current).unwrap().1.1,
            _ => None.unwrap(),
        }
    }
    println!("{}",step);
    
}

fn get_index (val: &str, target: &Vec<(String,(String,String))>)-> Option<usize>{
    for i in 0..target.len(){
        if val == target.get(i).unwrap().0 {
            return Some(i);
        }
    }
    return None;
}
