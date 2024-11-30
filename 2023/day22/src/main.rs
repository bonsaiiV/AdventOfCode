use regex::Regex;
use std::cmp::max;
use std::collections::HashSet;
use std::fs::File;
use std::io::{self, BufRead};
use std::iter::repeat;

#[derive(Debug)]
struct V3 {
    x: usize,
    y: usize,
    z: usize,
}

#[derive(Debug)]
struct Brick {
    label: usize,
    start: V3,
    end: V3,
    supported_by: HashSet<usize>,
}

fn main() {
    let re_brick = Regex::new(r"(\d+),(\d+),(\d+)~(\d+),(\d+),(\d+)").unwrap();
    let input_reader = io::BufReader::new(File::open("resources/input").unwrap());
    let mut bricks: Vec<Brick> = Vec::new();
    let mut line_nr = 1;
    let mut max_x = 0;
    let mut max_y = 0;
    for line_opt in input_reader.lines() {
        let line = line_opt.unwrap();
        let brick_cap = re_brick.captures(&line).unwrap();
        let brick = Brick {
            label: line_nr,
            start: V3 {
                x: brick_cap[1].parse::<usize>().unwrap(),
                y: brick_cap[2].parse::<usize>().unwrap(),
                z: brick_cap[3].parse::<usize>().unwrap(),
            },
            end: V3 {
                x: brick_cap[4].parse::<usize>().unwrap(),
                y: brick_cap[5].parse::<usize>().unwrap(),
                z: brick_cap[6].parse::<usize>().unwrap(),
            },
            supported_by: HashSet::new(),
        };
        max_x = max(max_x, brick.end.x + 1);
        max_y = max(max_y, brick.end.y + 1);
        bricks.push(brick);
        line_nr += 1;
    }
    let mut max_heights = repeat(repeat((0, 0)).take(max_y).collect::<Vec<_>>())
        .take(max_x)
        .collect::<Vec<_>>();
    bricks.sort_by_key(|brick| brick.start.z);
    for brick in bricks.iter_mut() {
        let mut ground = Vec::new();
        for x in brick.start.x..brick.end.x + 1 {
            for y in brick.start.y..brick.end.y + 1 {
                ground.push(max_heights[x][y]);
            }
        }
        let height = ground.iter().fold(0, |max_z, &(z, _)| max(max_z, z));
        brick.supported_by = ground
            .iter()
            .filter(|&(z, _)| *z == height)
            .map(|&(_, label)| label)
            .collect::<HashSet<_>>();
        for x in brick.start.x..brick.end.x + 1 {
            for y in brick.start.y..brick.end.y + 1 {
                max_heights[x][y] = (height + brick.end.z - brick.start.z + 1, brick.label);
            }
        }
    }
    let result1 = bricks
        .iter()
        .filter(|candidate| {
            !bricks.iter().any(|other_brick| {
                other_brick.supported_by.contains(&candidate.label)
                    && other_brick.supported_by.len() == 1
            })
        })
        .count();
    let mut total_fallen = 0;
    for brick in bricks.iter(){//.filter(|b| b.supported_by.contains(&0)) {
        let mut disintegrated = HashSet::from([brick.label]);
        for other_brick in bricks.iter(){
            if other_brick.supported_by.iter().all(|support| disintegrated.contains(support)){
                disintegrated.insert(other_brick.label);
            }
        }
        total_fallen += disintegrated.len() - 1;
    }
    println!("{}", result1);
    println!("{}", total_fallen);
    println!("total bricks: {}", bricks.len());
}
