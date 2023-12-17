use std::fs::File;
use std::io::{self, BufRead};
use std::collections::BinaryHeap;
use std::cmp::Ordering;
use std::collections::HashMap;


#[derive(Debug)]
struct HeatCell{
    heat_loss_map: HashMap<(char,usize),usize>,
    loss: usize,
}
#[derive(PartialEq, Debug, Eq)]
struct FrontPos{
    x: usize,
    y: usize,
    //history: Vec<(usize, usize)>,
    direction: char,
    straight_moved: usize,
    total_heat_loss: usize,
}
impl Ord for FrontPos {
    fn cmp(&self, other: &Self) -> Ordering {
        if other.x + other.y + self.total_heat_loss < self.x + self.y + other.total_heat_loss{
            return Ordering::Greater
        }
        else if other.x + other.y + self.total_heat_loss > self.x + self.y + other.total_heat_loss{
            return Ordering::Less
        }else {
            return Ordering::Equal
        }
    }
}
impl PartialOrd for FrontPos {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}
fn main() {
    let input_reader = io::BufReader::new(File::open("resources/input").unwrap());
    let mut height = 0;
    let mut heat_map: Vec<Vec<HeatCell>> = Vec::new();
    let mut width = 0;
    input_reader.lines().for_each(|line| {
        let line_str = line.unwrap();
        width = line_str.len();
        let mut next_line = Vec::new();
        for sym in line_str.chars(){
            next_line.push(HeatCell{loss: sym.to_digit(10).unwrap() as usize, heat_loss_map: HashMap::new()});
        }
        height += 1;
        heat_map.push(next_line);
    });
    println!("width: {}, height: {}", width, height);
    let mut front = BinaryHeap::new();
    front.push(FrontPos{x:0,y:0, direction: 'S', straight_moved: 0, total_heat_loss: 0});
    let mut cl = 0;
    //front.push(FrontPos{x:0,y:0, history: Vec::from([(0,0)]), direction: 'S', straight_moved: 0, total_heat_loss: 0});
    let mut maxfl = front.len();
    while heat_map[height-1][width-1].heat_loss_map.len() == 0 {
        if front.len() > maxfl {
            maxfl = front.len();
        }
        let tmp = front.peek().unwrap().total_heat_loss;
        if tmp > cl{
            cl = tmp;
            println!("{} with fl: {}", cl, front.len());
        }
        //println!("{:?}", front.peek());
        //println!("{:?}", front);
        unvisited_neighbors(front.pop().unwrap(), &mut heat_map, &mut front, width, height);
    }
    println!("{}", maxfl);
    println!("{}", heat_map[height-1][width-1].heat_loss_map.values().next().unwrap());
}
fn visit_neighbor(next_cell_pos: (usize,usize), heat_map: &mut Vec<Vec<HeatCell>>, direction: char, front: &mut BinaryHeap<FrontPos>, drop: &FrontPos){
    let mut moved = 1;
    if drop.direction == direction{
        if drop.straight_moved < 3 {
            moved = drop.straight_moved + 1;
        }else {
            return
        }
    }
    let next_cell = heat_map.get_mut(next_cell_pos.1).unwrap().get_mut(next_cell_pos.0).unwrap();
    for i in 1..moved+1{
        if let Some(prev_heat_loss) = next_cell.heat_loss_map.get(&(direction, i)){
            if *prev_heat_loss <= drop.total_heat_loss + next_cell.loss{
                return
            }
        }
    }
    //let mut new_history = drop.history.clone();
    //new_history.push(next_cell_pos);
    front.push(FrontPos{
        x: next_cell_pos.0, 
        y:next_cell_pos.1, 
        //history: new_history,
        direction: direction, 
        straight_moved: moved,
        total_heat_loss: drop.total_heat_loss + next_cell.loss});
    next_cell.heat_loss_map.insert((direction, moved),drop.total_heat_loss + next_cell.loss);
}
fn unvisited_neighbors(cell: FrontPos, heat_map: &mut Vec<Vec<HeatCell>>, front: &mut BinaryHeap<FrontPos>, width: usize, height: usize){
    //println!("{:?}", cell.history);
    if cell.direction != 'E' && cell.x > 0 {
        visit_neighbor((cell.x-1, cell.y), heat_map, 'W', front, &cell);
    }
    if cell.direction != 'S' && cell.y > 0 {
        visit_neighbor((cell.x, cell.y-1), heat_map, 'N', front, &cell);
    }
    if cell.direction != 'W' && cell.x < width-1 {
        visit_neighbor((cell.x+1, cell.y), heat_map, 'E', front, &cell);
    }
    if cell.direction != 'N' && cell.y < height-1 {
        visit_neighbor((cell.x, cell.y+1), heat_map, 'S', front, &cell);
    }
}
