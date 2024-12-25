#include "promote.h"
#include "internal.h"
#include "logging.h"

void kissat_promote_clause (kissat *solver, clause *c, unsigned new_glue) {
  if (!GET_OPTION (promote))
    return;
  assert (c->redundant);
  const unsigned old_glue = c->glue;
  assert (new_glue < old_glue);
  const unsigned tier1 = TIER1;
  const unsigned tier2 = MAX (TIER2, TIER1);
  if (old_glue <= tier1) {
    LOGCLS (c, "keeping with new glue %u in tier1", new_glue);
    INC (clauses_kept1);
  } else if (new_glue <= tier1) {
    assert (tier1 < old_glue);
    assert (new_glue <= tier1);
    LOGCLS (c, "promoting with new glue %u to tier1", new_glue);
    INC (clauses_promoted1);
  } else if (tier2 < old_glue && new_glue <= tier2) {
    assert (tier2 < old_glue);
    assert (tier1 < new_glue), assert (new_glue <= tier2);
    LOGCLS (c, "promoting with new glue %u to tier2", new_glue);
    INC (clauses_promoted2);
  } else if (old_glue <= tier2) {
    assert (tier1 < old_glue), assert (old_glue <= tier2);
    assert (tier1 < new_glue), assert (new_glue <= tier2);
    LOGCLS (c, "keeping with new glue %u in tier2", new_glue);
    INC (clauses_kept2);
  } else {
    assert (tier2 < old_glue);
    assert (tier2 < new_glue);
    LOGCLS (c, "keeping with new glue %u in tier3", new_glue);
    INC (clauses_kept3);
  }
  INC (clauses_improved);
  c->glue = new_glue;
#ifndef LOGGING
  (void) solver;
#endif
}
