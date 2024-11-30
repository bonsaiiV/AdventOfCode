use std::ops::{Add,Sub, AddAssign, Mul};
#[derive(Eq, Hash, Debug, Copy, Clone, PartialEq)]
pub struct V2 {
    pub x: isize,
    pub y: isize,
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
impl AddAssign for V2 {
    fn add_assign(&mut self, other: Self) {
        *self = Self {
            x: self.x + other.x,
            y: self.y + other.y,
        };
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
impl Mul<isize> for V2{
    type Output = Self;
    fn mul(self, other: isize) -> Self{
        Self {
            x: self.x * other,
            y: self.y * other,
        }
    }
}
/*impl V2{
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
}*/
pub const N: V2 = V2{x:0,y:-1};
pub const E: V2 = V2{x:1,y:0};
pub const S: V2 = V2{x:0,y:1};
pub const W: V2 = V2{x:-1,y:0};

