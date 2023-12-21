use regex::Regex;
use std::collections::{HashMap, VecDeque};
use std::fs::File;
use std::io::{self, BufRead};
pub trait PulseModule {
    fn apply_pulse(&mut self, pulse_high: bool, src: &String) -> Option<bool>;
    fn get_outgoing(&self) -> &Vec<String>;
    fn set_inputs(&mut self, _inputs: &Vec<String>) {
        panic!("Wrong struct")
    }
    fn get_str(&self) -> Option<char>{
        None
    }
}
#[derive(Debug)]
struct Broadcaster {
    outports: Vec<String>,
}
impl PulseModule for Broadcaster {
    fn apply_pulse(&mut self, pulse_high: bool, _src: &String) -> Option<bool> {
        Some(pulse_high)
    }
    fn get_outgoing(&self) -> &Vec<String> {
        &self.outports
    }
}
#[derive(Debug)]
struct FlipFlop {
    outports: Vec<String>,
    state: bool,
}
impl PulseModule for FlipFlop {
    fn apply_pulse(&mut self, pulse_high: bool, _src: &String) -> Option<bool> {
        if pulse_high {
            None
        } else {
            self.state = !self.state;
            Some(self.state)
        }
    }
    fn get_outgoing(&self) -> &Vec<String> {
        &self.outports
    }
    fn get_str(&self) -> Option<char>{
        if self.state {
            Some('1')
        }else {
            Some('0')
        }
    }
}
#[derive(Debug)]
struct Conjunction {
    outports: Vec<String>,
    state: HashMap<String, bool>,
}
impl PulseModule for Conjunction {
    fn apply_pulse(&mut self, pulse_high: bool, src: &String) -> Option<bool> {
        self.state.insert(src.to_owned(), pulse_high);
        if self.state.values().all(|high| *high) {
            Some(false)
        } else {//if self.state.values().all(|high| !*high) {
            Some(true)
        } /*else {
            None
        }*/
    }
    fn get_outgoing(&self) -> &Vec<String> {
        &self.outports
    }
    fn set_inputs(&mut self, inputs: &Vec<String>) {
        for input in inputs.iter() {
            self.state.insert(input.to_string(), false);
        }
    }
}
fn main() {
    let re_line = Regex::new(r"^(?P<name>.+) -> (?P<outports>.+)$").unwrap();
    let re_name = Regex::new(r"(?P<bc>broadcaster)|\%(?P<ff>[a-z]+)|\&(?P<con>[a-z]+)").unwrap();
    let input_reader = io::BufReader::new(File::open("resources/input").unwrap());
    let mut modules_map: HashMap<String, Box<dyn PulseModule>> = HashMap::new();
    let mut all_con = Vec::new();
    for line in input_reader.lines() {
        let line_str = line.unwrap();
        let line_cap = re_line.captures(&line_str).unwrap();
        let name_cap = re_name.captures(&line_cap["name"]).unwrap();
        let outports = line_cap["outports"]
            .split(", ")
            .map(|label| label.to_string())
            .collect::<Vec<_>>();
        if let Some(name) = name_cap.name("bc") {
            modules_map.insert(
                name.as_str().to_string(),
                Box::new(Broadcaster { outports: outports }),
            );
        } else if let Some(name) = name_cap.name("ff") {
            modules_map.insert(
                name.as_str().to_string(),
                Box::new(FlipFlop {
                    outports: outports,
                    state: false,
                }),
            );
        } else if let Some(name) = name_cap.name("con") {
            modules_map.insert(
                name.as_str().to_string(),
                Box::new(Conjunction {
                    outports: outports,
                    state: HashMap::new(),
                }),
            );
            all_con.push(name.as_str().to_string());
        }
    }
    let con_inports = all_con
        .iter()
        .map(|con_name| {
            (
                con_name,
                modules_map
                    .iter()
                    .filter(|(_, module)| module.get_outgoing().contains(con_name))
                    .map(|(name, _)| name.to_string())
                    .collect::<Vec<String>>(),
            )
        })
        .collect::<Vec<_>>();
    for (con_name, inports) in con_inports {
        modules_map.get_mut(con_name).unwrap().set_inputs(&inports);
    }
    let mut count_high = 0;
    let mut count_low = 1000;
    //for i in 0..1000 {
    let mut button_press = 0;
    let goals = vec!["fh", "mf", "fz", "ss"];
    while true {
        //println!("{}", modules_map.values().map(|modul| modul.get_str()).filter(|opt| opt.is_some()).map(|opt| opt.unwrap()).collect::<String>());
        button_press += 1;
        //println!("Press Button for the {} time:", i+1);
        let bc = "broadcaster".to_string();
        let init_empty = "button".to_string();
        let mut queue = VecDeque::from([(bc, false, init_empty)]);
        while let Some((next_module_name, pulse_high, from)) = queue.pop_front() {
            //println!("{}, {}, {}", from, pulse_high, next_module_name);
            if goals.contains(&&next_module_name.as_str()) && !pulse_high{
                println!("{}: {}", next_module_name, button_press);
            }
            if let Some(next_module) = modules_map.get_mut(&next_module_name) {
                let tmp_pulse = next_module.apply_pulse(pulse_high, &from);
                if let Some(new_pulse) = tmp_pulse {
                    for out in modules_map.get(&next_module_name).unwrap().get_outgoing() {
                        queue.push_back((out.clone(), new_pulse, next_module_name.clone()));
                        if new_pulse {
                            count_high += 1;
                        } else {
                            count_low += 1;
                        }
                    }
                }
            }
        }
    }
    println!("{}, {}", count_high, count_low);
    println!("{}", count_high * count_low);
}
