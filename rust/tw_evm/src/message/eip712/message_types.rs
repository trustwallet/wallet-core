// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::message::eip712::property::{Property, PropertyType};
use std::collections::hash_map::Entry;
use std::collections::HashMap;

pub type CustomTypes = HashMap<String, Vec<Property>>;

pub trait DeclareCustomType {
    /// A custom type may also depend on other custom types.
    fn declare_custom_type(&self, builder: &mut MessageTypesBuilder);
}

#[derive(Default)]
pub struct MessageTypesBuilder {
    types: CustomTypes,
}

impl MessageTypesBuilder {
    pub fn add_custom_type(&mut self, type_name: String) -> Option<CustomTypeBuilder> {
        match self.types.entry(type_name) {
            Entry::Vacant(entry) => {
                let type_properties = entry.insert(Vec::default());
                Some(CustomTypeBuilder { type_properties })
            },
            Entry::Occupied(_) => None,
        }
    }

    pub fn build(self) -> CustomTypes {
        self.types
    }
}

pub struct CustomTypeBuilder<'a> {
    type_properties: &'a mut Vec<Property>,
}

impl<'a> CustomTypeBuilder<'a> {
    pub fn add_property(&mut self, name: &str, property_type: PropertyType) -> &mut Self {
        self.type_properties.push(Property {
            name: name.to_string(),
            property_type: property_type.to_string(),
        });
        self
    }

    pub fn sort_by_names(&mut self) {
        self.type_properties.sort_by(|x, y| x.name.cmp(&y.name));
    }
}
