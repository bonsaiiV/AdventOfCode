use std::fs;
use std::collections::HashMap;
use regex::Regex;

#[derive(Debug)]
struct Workflow {
    steps: Vec<(String, bool, usize, String)>,
    end: String,
}
fn main() {
    let re_workflow = Regex::new(r"(?P<label>[a-z]+)\{(?P<steps>.*)\}").unwrap();
    let re_step = Regex::new(r"(?P<mid>[xmsa][<>]\d+\:[a-zRA]+),|(?P<end>[a-zRA]+)$").unwrap();
    let re_mid_step = Regex::new(r"(?P<cat>[xmsa])(?P<op>[<>])(?P<num>\d+)\:(?P<to>[a-zRA]+)$").unwrap();
    let re_part = Regex::new(r"\{x=(?P<cool>\d+),m=(?P<musical>\d+),a=(?P<aerodynamic>\d+),s=(?P<shiny>\d+)").unwrap();
    let input_string = fs::read_to_string("resources/input").unwrap();
    let (instructions, parts) = input_string.split_once("\n\n").unwrap();
    let mut workflows: HashMap<String, Workflow> = HashMap::new();
    for line in instructions.lines(){
        let workflow_cap = re_workflow.captures(&line).unwrap();
        let label = &workflow_cap["label"];
        let mut steps_vec = Vec::new();
        for cap in re_step.captures_iter(&workflow_cap["steps"]){
            //println!("{}", &cap[0]);
            if let Some(end) = cap.name("end"){
                workflows.insert(label.to_string(), Workflow{ steps: steps_vec.clone(), end:end.as_str().to_string()});
            }else {
                let this_cap = re_mid_step.captures(&cap["mid"]).unwrap();
                steps_vec.push((this_cap["cat"].to_string(),
                    &this_cap["op"] == ">",
                    this_cap["num"].parse::<usize>().unwrap(),
                    this_cap["to"].to_string()));
            }
        }
    }
    let mut result1 = 0;
    for line in parts.lines(){
        let part_cap = re_part.captures(&line).unwrap();
        let part = [("x", &part_cap["cool"]),
            ("m", &part_cap["musical"]),
            ("a", &part_cap["aerodynamic"]),
            ("s", &part_cap["shiny"])]
            .iter()
            .map(|(sym, score)| (sym.to_string(), score.parse::<usize>().unwrap()))
            .collect::<HashMap<_,_>>();
        let mut next_workflow_label = "in";
        //println!("{:?}", part);
        while !(next_workflow_label == "R" || next_workflow_label == "A"){
            let current_workflow = workflows.get(next_workflow_label).unwrap();
            //println!("{:?}",current_workflow);
            next_workflow_label = &current_workflow.end;
            for (cat, op_greater, num, to) in current_workflow.steps.iter().rev(){
                if *op_greater{
                    if part.get(cat).unwrap() > num {
                        next_workflow_label = to;
                    }
                }else {
                    if part.get(cat).unwrap() < num {
                        next_workflow_label = to;
                    }
                }
            }
            //println!("{}",next_workflow_label);
        }
        if next_workflow_label == "A"{
            result1 += part.values().sum::<usize>();
        }
    }
    println!("{}", result1);
    let mut front: Vec<(HashMap<String,(usize,usize)>, &str)> = Vec::new();
    let first_part = [("x", (0,4000)),
            ("m", (0,4000)),
            ("a", (0,4000)),
            ("s", (0,4000))]
            .iter()
            .map(|(sym, score)| (sym.to_string(), *score))
            .collect::<HashMap<_,_>>();
    front.push((first_part, "in"));
    let mut result2 = 0;
    while let Some((mut part, workflow_label)) = front.pop(){
        println!("{}", workflow_label);
        let current_workflow = workflows.get(workflow_label).unwrap();
        //println!("{:?}",current_workflow);
        for (cat, op_greater, num, to) in current_workflow.steps.iter(){
            if part.get(cat).unwrap().0 <= *num && part.get(cat).unwrap().1 >= *num {
                if *op_greater{
                    if to == "A"{
                        let mut new_part = part.clone();
                        new_part.insert(cat.to_string(), (*num, part.get(cat).unwrap().1));
                        result2 += new_part.values().map(|(start,end)| end-start).product::<usize>();
                    } else if to != "R"{
                        let mut new_part = part.clone();
                        new_part.insert(cat.to_string(), (*num, part.get(cat).unwrap().1));
                        front.push((new_part, to));
                    }
                    part.insert(cat.to_string(), (part.get(cat).unwrap().0, *num));
                }else {
                    if to == "A"{
                        let mut new_part = part.clone();
                        new_part.insert(cat.to_string(), (part.get(cat).unwrap().0, *num -1));
                        result2 += new_part.values().map(|(start,end)| end-start).product::<usize>();
                    } else if to != "R"{
                        let mut new_part = part.clone();
                        new_part.insert(cat.to_string(), (part.get(cat).unwrap().0, *num -1));
                        front.push((new_part, to));
                    }
                    part.insert(cat.to_string(), (*num-1, part.get(cat).unwrap().1));
                }
            }
        }
        let end = &current_workflow.end;
        if end =="A"{
            result2 += part.values().map(|(start,end)| end-start).product::<usize>();
        }else if end != "R"{
            front.push((part, end));
        }
    }
    println!("{}", result2);
}
