mod vector;
use crate::vector::*;
use std::io::{self, BufRead};
use std::fs::File;
use std::collections::{HashMap, HashSet};
use itertools::Itertools;
//use std::cmp::{min, max};

fn parse_color(color: &str) -> V2{
    let color_steps = color.chars().skip(2).take(5).map(|sym| sym.to_digit(16).unwrap() as usize).fold(0, |sum, digit| sum*16+digit);
    let last_sym = color.chars().nth(7).unwrap();
    let dir = match last_sym {
        '0' => E,
        '1' => S,
        '2' => W,
        '3' => N,
        _   => panic!("unexpected Symbol: {}", last_sym),
    };
    dir * (color_steps as isize)
}
fn main() {
    let input_reader = io::BufReader::new(File::open("resources/input").unwrap());
    let mut dig_map = HashMap::new();
    let mut dig_map2: HashSet<(V2, V2)> = HashSet::new();
    let mut current_pos = V2{x:0, y:0};
    let mut current_pos2 = V2{x:0, y:0};
    for line in input_reader.lines(){
        let line_str = line.unwrap();
        let (direction_str, rest)  = line_str.split_once(' ').unwrap();
        let (steps_str, color) = rest.split_once(' ').unwrap();
        // construct map for part1
        let steps = steps_str.parse().unwrap();
        let direction = get_dir_v2(direction_str);
        (0..steps).for_each(|_| {
            dig_map.insert(current_pos, 1);
            if !(dig_map.get(&(left(direction)+current_pos)).is_some()){
                dig_map.insert(left(direction)+current_pos, 2);
            }
            if !dig_map.get(&(right(direction)+current_pos)).is_some(){
                dig_map.insert(right(direction)+current_pos, 3);
            }
            current_pos += direction;
        });
        // construct map for part2
        let next_move = parse_color(color);
        let end_pos = current_pos2 + next_move;
        if end_pos.y == current_pos2.y{
            if current_pos2.x < end_pos.x{
                dig_map2.insert((current_pos2, end_pos));
            }else {
                dig_map2.insert((end_pos, current_pos2));
            }
        }
        current_pos2 = end_pos;
    }
    // solve part1
    let min_pos = dig_map.keys().fold(V2{x: 0, y:0}, |min_pos, next_pos| if min_pos.x < next_pos.x {min_pos} else {*next_pos}); 
    let inside_marker;
    if *dig_map.get(&min_pos).unwrap()==2 {
        inside_marker = 3;
    }else {
        inside_marker = 2;
    }
    let mut front = dig_map.iter().filter(|(_, marker)| **marker == inside_marker).map(|(pos, _)| *pos).collect::<Vec<V2>>();
    while !front.is_empty(){
        let next_pos = front.pop().unwrap();
        for dir in [N,E,S,W]{
            if !dig_map.contains_key(&(next_pos+dir)){
                dig_map.insert(next_pos+dir, inside_marker);
                front.push(next_pos+dir);
            }
        }
        dig_map.insert(next_pos,1);
    }
    println!("{}", dig_map.iter().filter(|(_, marker)| **marker == 1).count());
    // solve part2
    let mut in_ranges: Vec<(isize, isize)> = Vec::new();
    let mut rel_ypos = dig_map2.iter().map(|(start, _)| start.y).unique().collect::<Vec<_>>();
    rel_ypos.sort();
    let mut last_ypos = -10000000;
    let mut result2 = 0;
    for &ypos in rel_ypos.iter(){
        for in_range in in_ranges.iter_mut(){
            result2 += (ypos-last_ypos) * (in_range.1-in_range.0+1);
        }
        for section in dig_map2.iter().filter(|&(start,_)| start.y == ypos).map(|&(start,end)| (start.x, end.x)) {
            let mut new_ranges = Vec::new();
            let mut high_extension = None;
            let mut low_extension = None;
            let mut skip_section = false;
            for in_range in in_ranges.iter_mut(){
                if in_range.0 > section.1 || in_range.1 < section.0{
                    new_ranges.push(*in_range);
                }else if in_range.0 == section.1{
                    high_extension = Some(in_range.1-section.1);
                }else if in_range.1 == section.0{
                    low_extension = Some(in_range.0-section.0);
                }else if in_range.0 == section.0 && in_range.1 == section.1{
                    skip_section=true;
                    result2+= section.1-section.0+1;
                }else if in_range.1 == section.1{
                    new_ranges.push((in_range.0, section.0));
                    //low_extension = Some(in_range.0-section.0);
                    result2+= section.1-section.0;
                    skip_section=true;
                }else if in_range.0 == section.0{
                    new_ranges.push((section.1, in_range.1));
                    //high_extension = Some(in_range.1-section.1);
                    result2+= section.1-section.0;
                    skip_section=true;
                }else if in_range.0 < section.0 && in_range.1 > section.1{
                    new_ranges.push((section.1, in_range.1));
                    new_ranges.push((in_range.0, section.0));
                    result2+= section.1-section.0-1;
                    skip_section=true;
                }
            }
            if let Some(he) = high_extension{
                if let Some(le) = low_extension{
                    new_ranges.push((section.0+le, section.1+he));
                } else {
                    new_ranges.push((section.0, section.1+he));
                }
            }else if let Some(le) = low_extension{
                new_ranges.push((section.0 + le, section.1));
            }else if !skip_section {
                new_ranges.push(section);
            }
            in_ranges = new_ranges;
        }
        last_ypos = ypos;
    }
    println!("{}", result2);
}
fn right(dir: V2) -> V2{
    match dir {
        N => E,
        E => S,
        S => W,
        W => N,
        _   => panic!("unexpected Symbol {:?}", dir),
    }
}
fn left(dir: V2) -> V2{
    match dir {
        N => W,
        E => N,
        S => E,
        W => S,
        _   => panic!("unexpected Symbol {:?}", dir),
    }
}
fn get_dir_v2(direction: &str) -> V2{
    match direction {
        "U" => N,
        "R" => E,
        "D" => S,
        "L" => W,
        _   => panic!("unexpected Symbol {}", direction),
    }
}
