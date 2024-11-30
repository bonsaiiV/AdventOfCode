use std::collections::HashMap;
use std::fs::File;
use std::io::{self, BufRead};
use std::ops::{Add, Sub};

#[derive(Eq, Hash, Debug, Copy, Clone, PartialEq)]
struct V2 {
    x: isize,
    y: isize,
}
impl Add for V2{
    type Output = Self;
    fn add(self, other:Self) -> Self{
        Self {
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}
impl Sub for V2{
    type Output = Self;
    fn sub(self , other:Self) -> Self{
        Self {
            x: self.x - other.x,
            y: self.y - other.y,
        }
    }
}
impl V2{
    fn abs(&self) -> isize{
        isize::abs(self.x) + isize::abs(self.y)
    }
    fn is_on(&self, field_size: &Self) -> bool{
        if self.x < 0 || self.y < 0 || self.x >= field_size.x || self.y >= field_size.y{
            false
        }else {
            true
        }
    }
}
const N: V2 = V2{x:0,y:-1};
const E: V2 = V2{x:1,y:0};
const S: V2 = V2{x:0,y:1};
const W: V2 = V2{x:-1,y:0};

fn main() {
    let input_reader = io::BufReader::new(File::open("resources/input").unwrap());
    let mut mirrors = HashMap::new();
    let mut line_nr = 0;
    let mut width = 0;
    for line in input_reader.lines(){
        let mut sym_pos = 0;
        let line_str = line.unwrap();
        for sym in line_str.chars(){
            match sym {
                '.' => (),
                mirror => {
                    mirrors.insert(V2{x:sym_pos, y:line_nr}, mirror);
                },
            }
            sym_pos += 1;
        }
        line_nr += 1;
        width = sym_pos;
    }
    let field_size = V2{x: width, y:line_nr};
    let result1 = get_number_energized(&mirrors, &field_size, (V2{x:0, y:0}, E));
    let mut result2 = 0;
    for i in 0..field_size.x{
        let tmp_res = get_number_energized(&mirrors, &field_size, (V2{x:i, y:0}, S));
        if tmp_res > result2{
            result2 = tmp_res;
        }
        let tmp_res = get_number_energized(&mirrors, &field_size, (V2{x:i, y:field_size.y-1}, N));
        if tmp_res > result2{
            result2 = tmp_res;
        }
    }
    for i in 0..field_size.y{
        let tmp_res = get_number_energized(&mirrors, &field_size, (V2{x:0, y:i}, E));
        if tmp_res > result2{
            result2 = tmp_res;
        }
        let tmp_res = get_number_energized(&mirrors, &field_size, (V2{y:i, x:field_size.x-1}, W));
        if tmp_res > result2{
            result2 = tmp_res;
        }
    }
    println!("{}", result1);
    println!("{}", result2);
}
fn get_number_energized(mirrors: &HashMap<V2, char>, field_size: &V2, start: (V2, V2)) -> usize {
    let mut front = Vec::from([(start)]);
    let mut next_directions;
    let mut energized:HashMap<V2, Vec<V2>> = HashMap::new();
    energized.insert(start.0, Vec::from([start.1]));
    while let Some((pos, direction)) = front.pop(){
        if let Some(mirror) = mirrors.get(&pos){
            next_directions = match mirror {
                '-' => match direction{
                    E | W => Vec::from([direction]),
                    _ => Vec::from([E,W]),
                },
                '|' => match direction{
                    N | S => Vec::from([direction]),
                    _ => Vec::from([N,S]),
                },
                '/' => match direction {
                    N => Vec::from([E]),
                    E => Vec::from([N]),
                    S => Vec::from([W]),
                    _ => Vec::from([S]),
                },
                _ => match direction {
                    N => Vec::from([W]),
                    E => Vec::from([S]),
                    S => Vec::from([E]),
                    _ => Vec::from([N]),
                },
            }
        }else {
            next_directions = Vec::from([direction]);
        }
        for dir in next_directions{
            let next_pos = pos + dir;
            if next_pos.is_on(&field_size){
                if let Some(energized_dirs) = energized.get_mut(&next_pos){
                    if energized_dirs.contains(&dir) {
                        continue;
                    }else {
                        energized_dirs.push(dir);
                    }
                }else {
                    energized.insert(next_pos, Vec::from([dir]));
                }
                front.push((next_pos, dir));
            }
        }
    }
    energized.keys().count()
}
