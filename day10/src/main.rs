use std::fs;
use std::collections::HashMap;

fn main() {
    let input_str = fs::read_to_string("resources/input").unwrap();
    let pos_to_pipe_map = input_str.lines()
        .enumerate()
        .map(|(line_nr, line)| 
            line.chars()
                .enumerate()
                .map(move |(char_pos,pipe_segment)| 
                    ((char_pos, line_nr), pipe_segment)
                )
        ).flatten()
        .collect::<HashMap<(usize,usize),char>>();
    let mut area_map = pos_to_pipe_map.iter()
        .map(|(pos, _)| (pos.clone(), 0))
        .collect::<HashMap<(usize,usize),usize>>();
    let mut current_pos = pos_to_pipe_map.iter()
        .filter(|(_, pipe_segment)| **pipe_segment=='S')
        .nth(0).unwrap().0.clone();
    let mut current_direction = 'N';
    area_map.insert(current_pos, 1);
    current_pos = (current_pos.0, current_pos.1 - 1);
    let mut current_pipe = pos_to_pipe_map.get(&current_pos).unwrap();
    let mut cycle_len = 1;
    let mut turns = (0,0);
    let mut next_turn;
    while *current_pipe != 'S'{
        cycle_len += 1;
        area_map.insert(current_pos, 1);
        (current_direction, next_turn) = next_move(current_pipe, &current_direction);
        match next_turn{
            (0,0) => match current_direction {
                'N' => if *area_map.get(&(current_pos.0-1, current_pos.1)).unwrap() == 0{
                    area_map.insert((current_pos.0-1, current_pos.1), 2);
                },
                'E' => if *area_map.get(&(current_pos.0, current_pos.1-1)).unwrap() == 0{
                    area_map.insert((current_pos.0, current_pos.1-1), 2);
                },
                'S' => if *area_map.get(&(current_pos.0+1, current_pos.1)).unwrap() == 0{
                    area_map.insert((current_pos.0+1, current_pos.1), 2);
                },
                'W' => if *area_map.get(&(current_pos.0, current_pos.1+1)).unwrap() == 0{
                    area_map.insert((current_pos.0, current_pos.1+1), 2);
                },
                _ => panic!("spinnning"),
            }
            (0,1) => match current_direction {
                'N' => {
                    if *area_map.get(&(current_pos.0-1, current_pos.1)).unwrap() == 0{
                        area_map.insert((current_pos.0-1, current_pos.1), 2);
                    }
                    if *area_map.get(&(current_pos.0, current_pos.1+1)).unwrap() == 0{
                        area_map.insert((current_pos.0, current_pos.1+1), 2);
                    }
                },
                'E' => {
                    if *area_map.get(&(current_pos.0, current_pos.1-1)).unwrap() == 0{
                        area_map.insert((current_pos.0, current_pos.1-1), 2);
                    }
                    if *area_map.get(&(current_pos.0-1, current_pos.1)).unwrap() == 0{
                        area_map.insert((current_pos.0-1, current_pos.1), 2);
                    }
                },
                'S' => {
                    if *area_map.get(&(current_pos.0+1, current_pos.1)).unwrap() == 0{
                        area_map.insert((current_pos.0+1, current_pos.1), 2);
                    }
                    if *area_map.get(&(current_pos.0, current_pos.1-1)).unwrap() == 0{
                        area_map.insert((current_pos.0, current_pos.1-1), 2);
                    }
                },
                'W' => {
                    if *area_map.get(&(current_pos.0, current_pos.1+1)).unwrap() == 0{
                        area_map.insert((current_pos.0, current_pos.1+1), 2);
                    }
                    if *area_map.get(&(current_pos.0+1, current_pos.1)).unwrap() == 0{
                        area_map.insert((current_pos.0+1, current_pos.1), 2);
                    }
                },
                _ => panic!("spinnning"),
            },
            (1,0) => (),
            _ => panic!("spinnning"),
        }
        current_pos = match current_direction {
            'N' => (current_pos.0, current_pos.1 - 1),
            'E' => (current_pos.0 + 1, current_pos.1),
            'S' => (current_pos.0, current_pos.1 + 1),
            'W' => (current_pos.0 - 1, current_pos.1),
            _   => panic!("invalid direction"),
        };
        current_pipe = pos_to_pipe_map.get(&current_pos).unwrap();
    }
    println!("{}", cycle_len/2);

    let mut front = area_map.iter()
        .filter(|(_, type_here)| **type_here == 2)
        .map(|(pos,_)| pos.clone())
        .collect::<Vec<_>>();
    let mut result2 = 0;
    while let Some(current_pos) = front.pop(){
        result2 += 1;
        for next_pos in [(current_pos.0+0,current_pos.1+1), (current_pos.0+1,current_pos.1), (current_pos.0,current_pos.1-1) , (current_pos.0-1,current_pos.1)]{

            if *area_map.get(&next_pos).unwrap() == 0{
                area_map.insert(next_pos, 2);
                front.push(next_pos);
            }
        }
    }
    println!("{:?}", result2);
}

fn next_move(pipe_segment: &char, last_move: &char) -> (char, (usize, usize)){
    match last_move {
        'N' => match pipe_segment {
            '|' => ('N', (0,0)),
            'F' => ('E', (0, 1)),
            '7' => ('W', (1, 0)),
            _   => panic!("unexpected Symbol {}", pipe_segment),
        }
        'E' => match pipe_segment {
            '-' => ('E', (0, 0)),
            'J' => ('N', (1, 0)),
            '7' => ('S', (0, 1)),
            _   => panic!("unexpected Symbol {}", pipe_segment),
        }
        'S' => match pipe_segment {
            '|' => ('S', (0, 0)),
            'L' => ('E', (1, 0)),
            'J' => ('W', (0, 1)),
            _   => panic!("unexpected Symbol {}", pipe_segment),
        }
        'W' => match pipe_segment {
            '-' => ('W', (0, 0)),
            'F' => ('S', (1, 0)),
            'L' => ('N', (0, 1)),
            _   => panic!("unexpected Symbol {}", pipe_segment),
        }
        _   => panic!("unexpected Direction {}", last_move),
    }

}
