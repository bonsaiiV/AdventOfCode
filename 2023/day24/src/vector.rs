use num::{abs, Signed};
use std::ops::{Add, AddAssign, Div, Mul, Neg, Sub};
#[derive(Debug, Copy, Clone, PartialEq)]
pub struct V3<T: Copy> {
    pub x: T,
    pub y: T,
    pub z: T,
}

impl<T> Neg for V3<T>
where
    T: Copy + Neg<Output = T>,
{
    type Output = Self;
    fn neg(self) -> Self {
        Self {
            x: -self.x,
            y: -self.y,
            z: -self.z,
        }
    }
}
impl<T> Add for V3<T>
where
    T: Copy + Add<Output = T>,
{
    type Output = Self;
    fn add(self, other: Self) -> Self {
        Self {
            x: self.x + other.x,
            y: self.y + other.y,
            z: self.z + other.z,
        }
    }
}
impl<T> AddAssign for V3<T>
where
    T: Copy + Add<Output = T>,
{
    fn add_assign(&mut self, other: Self) {
        *self = Self {
            x: self.x + other.x,
            y: self.y + other.y,
            z: self.z + other.z,
        };
    }
}
impl<T> Sub for V3<T>
where
    T: Copy + Sub<Output = T>,
{
    type Output = V3<T>;
    fn sub(self, other: Self) -> Self {
        Self {
            x: self.x - other.x,
            y: self.y - other.y,
            z: self.z - other.z,
        }
    }
}
impl<T> Div<T> for V3<T>
where
    T: Copy + Div<Output = T>,
{
    type Output = Self;
    fn div(self, other: T) -> Self {
        Self {
            x: self.x / other,
            y: self.y / other,
            z: self.z / other,
        }
    }
}
impl<T> Mul<T> for V3<T>
where
    T: Copy + Mul<Output = T>,
{
    type Output = Self;
    fn mul(self, other: T) -> Self {
        Self {
            x: self.x * other,
            y: self.y * other,
            z: self.z * other,
        }
    }
}
impl<T> V3<T>
where
    T: Copy + Signed,
{
    pub fn manhatten(&self) -> T {
        num::abs(self.x) + abs(self.y) + abs(self.z)
    }
}
