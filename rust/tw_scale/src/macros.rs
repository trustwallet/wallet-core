use super::*;

macro_rules! tuple_impl {
    ($(,)?) => {};
    ($first:tt, $($rest:tt),* $(,)?) => {
        tuple_impl!($($rest),*,);
        tuple_impl!(@make_impl $first $($rest)*);
    };
    (@make_impl $($t:tt)+) => {
        #[allow(non_snake_case, unused_parens, unconditional_recursion)] // the compiler seems confused here
        impl <$($t),+> ToScale for ($($t),+,) where $($t: ToScale),+ {
            fn to_scale_into(&self, out: &mut Vec<u8>) {
                let ($($t),*) = self;
                $(
                    $t.to_scale_into(out);
                )*
            }
        }
    };
}

//tuple_impl!(T0, T1, T2, T3, T4, T5, T6, T7);
tuple_impl!(T0, T1, T2);

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
      $($unparsed_variants:tt)*
    }
    $($rest:tt)*
  ) => {
    $crate::impl_enum_scale! {
      @normalize
      enum {
        // Enum type
        {
          $(#[$enum_meta])*
          pub enum $enum_name
        }
        // Enum variants.
        []
        // Variant counter.
        [0]
        // Variant encoding.
        []
        __internal_unparsed_variants {
            $($unparsed_variants)*
        }
      }
    }
  };
  // Parse enum variants wihtout custom index.
  (@normalize
    enum {
      // Enum type
      { $( $enum_type:tt )* }
      // Enum variants.
      [ $( $enum_variants:tt )* ]
      // Variant counter.
      [ $( $count_variant:tt )* ]
      // Variant encoding.
      [ $( $encode_variants:tt )* ]
      __internal_unparsed_variants {
        $(#[$variant_meta:meta])*
        $variant_name:ident $(($variant_tuple_ty:ty))? $({
          $(
            $variant_field_name:ident : $variant_field_ty:ty
          ),+ $(,)?
        })?,
        $($unparsed_variants:tt)*
      }
    }
  ) => {
    $crate::impl_enum_scale! {
      @normalize
      enum {
        // Enum type
        { $( $enum_type )* }
        // Enum variants.
        [
          $( $enum_variants )*
          $(#[$variant_meta])*
          $variant_name $(($variant_tuple_ty))? $({
            $(
              $variant_field_name : $variant_field_ty
            ),+
          })?,
        ]
        // Variant counter.
        [ $( $count_variant )* + 1 ]
        // Variant encoding.
        [
          $( $encode_variants )*
          {
            ( $( $count_variant )* ): $variant_name $(($variant_tuple_ty))? $({
              $(
                $variant_field_name
              ),+
            })?
          }
        ]
        __internal_unparsed_variants {
            $($unparsed_variants)*
        }
      }
    }
  };
  // Parse enum variants with custom index.
  (@normalize
    enum {
      // Enum type
      { $( $enum_type:tt )* }
      // Enum variants.
      [ $( $enum_variants:tt )* ]
      // Variant counter.
      [ $( $count_variant:tt )* ]
      // Variant encoding.
      [ $( $encode_variants:tt )* ]
      __internal_unparsed_variants {
        $(#[$variant_meta:meta])*
        $variant_name:ident $(($variant_tuple_ty:ty))? $({
          $(
            $variant_field_name:ident : $variant_field_ty:ty
          ),+ $(,)?
        })? = $index:expr,
        $($unparsed_variants:tt)*
      }
    }
  ) => {
    $crate::impl_enum_scale! {
      @normalize
      enum {
        // Enum type
        { $( $enum_type )* }
        // Enum variants.
        [
          $( $enum_variants )*
          $(#[$variant_meta])*
          $variant_name $(($variant_tuple_ty))? $({
            $(
              $variant_field_name : $variant_field_ty
            ),+
          })? = $index,
        ]
        // Variant counter.
        [ $index + 1 ]
        // Variant encoding.
        [
          $( $encode_variants )*
          {
            ( $index ): $variant_name $(($variant_tuple_ty))? $({
              $(
                $variant_field_name
              ),+
            })?
          }
        ]
        __internal_unparsed_variants {
            $($unparsed_variants)*
        }
      }
    }
  };
  // Finished parsing enum variants.
  (@normalize
    enum {
      // Enum type
      {
        $(#[$enum_meta:meta])*
        pub enum $enum_name:ident
      }
      // Enum variants.
      [ $( $enum_variants:tt )* ]
      // Variant counter.
      [ $( $count_variant:tt )* ]
      // Variant encoding.
      [
        $(
          {
            ( $( $variant_index:tt )* ): $variant_name:ident $(($variant_tuple_ty:ty))? $({
              $(
                $variant_field_name:ident
              ),+
            })?
          }
        )*
      ]
      __internal_unparsed_variants { }
    }
  ) => {
    $(#[$enum_meta])*
    #[repr(u8)]
    pub enum $enum_name {
      $( $enum_variants)*
    }

    impl $crate::ToScale for $enum_name {
      fn to_scale_into(&self, out: &mut Vec<u8>) {
        match self {
          $(
            Self::$variant_name $(($crate::replace_ident!($variant_tuple_ty p0)))? $({
              $($variant_field_name),+
            })? => {
              // variant index.
              out.push(($( $variant_index)*) as u8);
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
    use super::*;

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
            Variant0,
            Variant1(u8),
            Variant10 = 10,
            Struct {
                id: u8,
                id2: u8,
            },
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
