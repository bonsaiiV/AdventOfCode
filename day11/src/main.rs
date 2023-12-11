use std::fs;
use std::ops::{Add, Sub};
use std::hash::Hash;
use std::cmp::max;

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
}
fn main() {
    let input_str = fs::read_to_string("resources/input").unwrap();
    let galaxy_positions = input_str.lines()
        .enumerate()
        .map(|(line_nr, line)| 
            line.chars()
                .enumerate()
                .map(move |(char_pos,symbol)| 
                    (V2 {x: usize::try_into(char_pos).unwrap(), y: usize::try_into(line_nr).unwrap()}, symbol)
                )
        ).flatten()
        .filter(|(_, symbol)| *symbol == '#')
        .map(|(pos, _)| pos)
        .collect::<Vec<_>>();
    let (width, height) = galaxy_positions.iter()
        .fold((0,0), |(w,h), pos| (max(w, pos.x), max(h, pos.y)));
    let mut result1: usize = (0..width).filter(|column_nr| 
        !galaxy_positions.iter()
            .any(|pos| *column_nr == pos.x))
        .map(|column_nr|
            galaxy_positions.iter()
                .filter(|pos| pos.x < column_nr)
                .count() *
            galaxy_positions.iter()
                .filter(|pos| pos.x > column_nr)
                .count())
        .sum::<usize>();
    result1 += (0..height).filter(|row_nr| 
        !galaxy_positions.iter()
            .any(|pos| *row_nr == pos.y))
        .map(|row_nr|
            galaxy_positions.iter()
                .filter(|pos| pos.y < row_nr)
                .count() *
            galaxy_positions.iter()
                .filter(|pos| pos.y > row_nr)
                .count())
        .sum::<usize>();
    let mut result2 = result1*999999;
    let galaxy_dists = <isize as TryInto<usize>>::try_into(galaxy_positions.iter()
        .map(|pos1| 
            galaxy_positions.iter()
                .map(|pos2| (*pos1-*pos2).abs())
                .sum::<isize>())
        .sum::<isize>()).unwrap()/2;
    result1 += galaxy_dists;
    result2 += galaxy_dists;
    
    println!("{}", result1);
    println!("{}", result2);
}
