jQuery( document ).ready(function() {
  /*
  window.alert( jQuery("#edit-toggle-display").prop( "checked" ) );
  */
  if( jQuery("#edit-toggle-display").prop( "checked" ) ) {
    jQuery(".toggle-display").show();
  }else {
    jQuery(".toggle-display").hide();
  }

  jQuery("#edit-toggle-display").on("click", function() {
    jQuery(".toggle-display").toggle();
  });

});
