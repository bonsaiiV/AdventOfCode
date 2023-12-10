use std::fs;
use std::ops::Add;
use std::hash::Hash;
use std::collections::HashMap;

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
const N: V2 = V2{x:0,y:-1};
const E: V2 = V2{x:1,y:0};
const S: V2 = V2{x:0,y:1};
const W: V2 = V2{x:-1,y:0};
fn main() {
    let input_str = fs::read_to_string("resources/input").unwrap();
    let pos_to_pipe_map = input_str.lines()
        .enumerate()
        .map(|(line_nr, line)| 
            line.chars()
                .enumerate()
                .map(move |(char_pos,pipe_segment)| 
                    (V2 {x: usize::try_into(char_pos).unwrap(), y: usize::try_into(line_nr).unwrap()}, pipe_segment)
                )
        ).flatten()
        .collect::<HashMap<V2,char>>();
    let mut area_map = pos_to_pipe_map.iter()
        .map(|(pos, _)| (pos.clone(), 0))
        .collect::<HashMap<V2,usize>>();
    let mut current_pos = pos_to_pipe_map.iter()
        .filter(|(_, pipe_segment)| **pipe_segment=='S')
        .nth(0).unwrap().0.clone();
    let mut current_direction = N;
    area_map.insert(current_pos, 1);
    current_pos = current_pos + V2 {x:0,y:-1};
    let mut current_pipe = pos_to_pipe_map.get(&current_pos).unwrap();
    let mut cycle_len = 1;
    //let mut turns = (0,0); this was used to figure out, whether i need the right or left side of
    //the path(i needed the left)
    let mut next_turn;
    while *current_pipe != 'S'{
        cycle_len += 1;
        area_map.insert(current_pos, 1);
        (current_direction, next_turn) = next_move(current_pipe, &current_direction);
        for candidate_pos in match next_turn{
            //theses depend on whether you need left or right side
            //to change you need to
            //(0,0): flip sign
            //(0,1): becomes Vec::new()
            //(1,0): becomes what (0,1) was except first value changes sign
            (0,0) => Vec::from(match current_direction {
                N => [W],
                E => [N],
                S => [E],
                W => [S],
                _ => panic!("spinnning"),
            }),
            (0,1) => Vec::from(match current_direction {
                N => [W, S],
                E => [N, W],
                S => [E, N],
                W => [S, E],
                _ => panic!("spinnning"),
            }),
            (1,0) => Vec::new(),
            _ => panic!("spinnning"),
        }{
            if *area_map.get(&(current_pos + candidate_pos)).unwrap() == 0{
                area_map.insert(current_pos + candidate_pos, 2);
            }
        }
        current_pos = current_pos + current_direction;
        current_pipe = pos_to_pipe_map.get(&current_pos).unwrap();
    }
    println!("{}", cycle_len/2);

    let mut front = area_map.iter()
        .filter(|(_, type_here)| **type_here == 2)
        .map(|(pos,_)| pos.clone())
        .collect::<Vec<_>>();
    let mut result2 = 0;
    let neighbors = [N, E, S, W];
    while let Some(current_pos) = front.pop(){
        result2 += 1;
        for neighbor in neighbors{
            let next_pos = current_pos + neighbor;
            if *area_map.get(&next_pos).unwrap() == 0{
                area_map.insert(next_pos, 2);
                front.push(next_pos);
            }
        }
    }
    println!("{:?}", result2);
}

fn next_move(pipe_segment: &char, last_move: &V2) -> (V2, (usize, usize)){
    match *last_move {
        N => match pipe_segment {
            '|' => (N, (0,0)),
            'F' => (E, (0, 1)),
            '7' => (W, (1, 0)),
            _   => panic!("unexpected Symbol {}", pipe_segment),
        }
        E => match pipe_segment {
            '-' => (E, (0, 0)),
            'J' => (N, (1, 0)),
            '7' => (S, (0, 1)),
            _   => panic!("unexpected Symbol {}", pipe_segment),
        }
        S => match pipe_segment {
            '|' => (S, (0, 0)),
            'L' => (E, (1, 0)),
            'J' => (W, (0, 1)),
            _   => panic!("unexpected Symbol {}", pipe_segment),
        }
        W => match pipe_segment {
            '-' => (W, (0, 0)),
            'F' => (S, (1, 0)),
            'L' => (N, (0, 1)),
            _   => panic!("unexpected Symbol {}", pipe_segment),
        }
        _   => panic!("unexpected Direction {:?}", last_move),
    }
}
