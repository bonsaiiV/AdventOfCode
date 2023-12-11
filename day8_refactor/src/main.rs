mod part1;
use regex::Regex;
use std::fs;
use std::str::Chars;
use std::iter::Cycle;
use std::collections::HashMap;

fn main() {
    let re = Regex::new(r"([A-Z]{3}) = \(([A-Z]{3}), ([A-Z]{3})\)").unwrap();
    let input_str = fs::read_to_string("resources/input").unwrap();
    let (instructions, mappings) = input_str.split_once("\n\n").unwrap();
    let instruction_len = instructions.len();
    let mut instructions_cycle = instructions.chars().cycle();
    let nodes_map = mappings.lines()
        .map(|line_str| {
            let (_, [node, left, right]) = re.captures(&line_str).unwrap().extract();
            (node.to_owned(), (left.to_owned(),right.to_owned()))})
        .collect::<HashMap<String, (String,String)>>();
    part1::solve(&mut instructions_cycle, &nodes_map);
    part2(&mut instructions_cycle, &nodes_map, instructions_len);
}
fn part2(instruction_cylce: &mut  Cycle<Chars<'_>>, nodes_map: &HashMap<String, (String,String)>, instructions_len: usize) {
    let ends = nodes_map.keys().filter(|node| (**node).chars().last().unwrap() == 'Z').collect::<Vec<_>>();
    let starts = nodes_map
        .keys()
        .filter(|node| (**node).chars().last().unwrap() == 'A')
        .map(|start| {
            
        }).fold((0, 1), |(prev_offset, prev_lcm), (current_offset, current_cycle)| (current_offset+prev_offset, current_cycle,prec_lcm)); 
}
