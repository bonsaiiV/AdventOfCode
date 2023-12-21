mod vector;
use crate::vector::{E, N, S, V2, W};
use std::collections::{HashMap, HashSet};
use std::fs;

#[derive(Debug)]
struct Field {
    is_active: bool,
    count_prev: usize,
    count_current: usize,
}

fn main() {
    let input_str = fs::read_to_string("resources/input5").unwrap();
    let steps = 64; //26501365;
    let mut line_nr = 0;
    let mut starting_pos = V2 { x: 0, y: 0 };
    let mut rocks: HashSet<V2> = HashSet::new();
    let mut width = 0;
    for line in input_str.lines() {
        let mut char_nr = 0;
        for sym in line.chars() {
            match sym {
                'S' => {
                    starting_pos = V2 {
                        y: line_nr,
                        x: char_nr,
                    }
                }
                '#' => {
                    rocks.insert(V2 {
                        y: line_nr,
                        x: char_nr,
                    });
                    ()
                }
                _ => (),
            };
            char_nr += 1;
        }
        width = char_nr;
        line_nr += 1;
    }
    let field_size = V2 {
        x: width,
        y: line_nr,
    };
    let mut garden_even = 0;
    let mut garden_odd = 0;
    println!("{}, {}", garden_even, garden_odd);
    println!("{:?}", field_size);
    let mut active_pos = HashSet::from([starting_pos]);
    for _ in 0..steps {
        active_pos = active_pos
            .into_iter()
            .map(|pos| get_neighbors(pos, &rocks, field_size))
            .flatten()
            .collect::<HashSet<V2>>();
    }
    println!("{:?}", active_pos.len());
}

fn get_neighbors(pos: V2, rocks: &HashSet<V2>, field_size: V2) -> Vec<V2> {
    let mut ret = Vec::new();

    for mov in [N, E, S, W] {
        let candidate = pos + mov;
        if candidate.is_on(&field_size) && !rocks.contains(&candidate) {
            ret.push(candidate);
        }
    }
    ret
}
