#[macro_export]
macro_rules! replace_ident {
    ($_t:tt $sub:ident) => {
        $sub
    };
}

#[macro_export]
macro_rules! replace_expr {
    ($_t:tt $sub:expr) => {
        $sub
    };
}

#[macro_export]
macro_rules! impl_struct_scale {
  // "New Type" struct
  (
    $(#[$struct_meta:meta])*
    pub struct $struct_name:ident (
      $(#[$struct_field_meta:meta])*
      $struct_field_vis:vis $struct_field_ty:ty
    );
  ) => {
    $(#[$struct_meta])*
    pub struct $struct_name(
      $(#[$struct_field_meta])*
      $struct_field_vis $struct_field_ty
    );

    impl $crate::ToScale for $struct_name {
      fn to_scale_into(&self, out: &mut Vec<u8>) {
        self.0.to_scale_into(out);
      }
    }
  };
  // Normal named fields struct.
  ($(#[$struct_meta:meta])*
  pub struct $struct_name:ident {
    $(
      $(#[$struct_field_meta:meta])*
      $struct_field_vis:vis $struct_field_name:ident : $struct_field_ty:ty
    ),+ $(,)?
  }) => {
    $(#[$struct_meta])*
    pub struct $struct_name {
      $(
        $(#[$struct_field_meta])*
        $struct_field_vis $struct_field_name : $struct_field_ty
      ),+
    }

    impl $crate::ToScale for $struct_name {
      fn to_scale_into(&self, out: &mut Vec<u8>) {
        $(self.$struct_field_name.to_scale_into(out);)+
      }
    }
  }
}

#[macro_export]
macro_rules! impl_enum_scale {
  (
    $(#[$enum_meta:meta])*
    pub enum $enum_name:ident {
      $(
        $(#[$variant_meta:meta])*
        $variant_name:ident $(($variant_tuple_ty:ty))? $({
          $(
            $variant_field_name:ident : $variant_field_ty:ty
          ),+ $(,)?
        })? = $variant_index:expr,
      )*
    }
    $($rest:tt)*
  ) => {
    $(#[$enum_meta])*
    #[repr(u8)]
    pub enum $enum_name {
      $(
          $(#[$variant_meta])*
          $variant_name $(($variant_tuple_ty))? $({
            $(
              $variant_field_name : $variant_field_ty
            ),+
          })? = $variant_index,
      )*
    }

    impl $crate::ToScale for $enum_name {
      fn to_scale_into(&self, out: &mut Vec<u8>) {
        match self {
          $(
            Self::$variant_name $(($crate::replace_ident!($variant_tuple_ty p0)))? $({
              $($variant_field_name),+
            })? => {
              // variant index.
              out.push($variant_index as u8);
              // Encode tuple variants.
              $(
                $crate::replace_expr!($variant_tuple_ty {
                  p0.to_scale_into(out);
                });
              )?
              // Encode struct variants.
              $(
                $($variant_field_name.to_scale_into(out);)+
              )?
            }
          )*
        }
      }
    }
  }
}

#[cfg(test)]
mod tests {
    use crate::ToScale;

    impl_struct_scale!(
        #[derive(Debug, Default, Clone, Ord, PartialOrd, Eq, PartialEq)]
        pub struct TestStruct {
            id: u8,
            id2: u8,
            data: Vec<u8>,
        }
    );

    #[test]
    fn test_struct_scale() {
        assert_eq!(
            TestStruct {
                id: 1,
                id2: 2,
                data: vec![3],
            }
            .to_scale(),
            &[0x01, 0x02, 0x04, 0x03]
        );
    }

    impl_struct_scale!(
        #[derive(Debug, Default, Clone, Ord, PartialOrd, Eq, PartialEq)]
        pub struct TestNewType(u8);
    );

    #[test]
    fn test_new_type_struct_scale() {
        assert_eq!(TestNewType(1).to_scale(), &[0x01]);
    }

    impl_enum_scale!(
        #[derive(Debug, Default, Copy, Clone, Ord, PartialOrd, Eq, PartialEq)]
        pub enum TestEnum {
            #[default]
            Variant0 = 0,
            Variant1(u8) = 1,
            Variant10 = 10,
            Struct {
                id: u8,
                id2: u8,
            } = 11,
        }
    );

    #[test]
    fn test_enum_scale() {
        assert_eq!(TestEnum::Variant0.to_scale(), &[0x00]);
        assert_eq!(TestEnum::Variant1(2).to_scale(), &[0x01, 0x02]);
        assert_eq!(TestEnum::Variant10.to_scale(), &[0x0A]);
        assert_eq!(
            TestEnum::Struct { id: 1, id2: 2 }.to_scale(),
            &[0x0B, 0x01, 0x02]
        );
    }
}
