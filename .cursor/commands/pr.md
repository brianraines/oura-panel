Create a pull request with thorough release notes in the description. Follow these steps:

1. **Determine the base branch**: Check the current branch and determine the base branch (typically `main` or `master`). If unsure, use `main` as default.

2. **Analyze commits**: Get all commits since the base branch using:
   ```bash
   git log main..HEAD --pretty=format:"%H|%s|%b" --reverse
   ```
   Or if the base branch is different:
   ```bash
   git log $(git merge-base main HEAD)..HEAD --pretty=format:"%H|%s|%b" --reverse
   ```

3. **Generate release notes**: Parse the commits following the Conventional Commits format (with emojis) from rule 200-conventional-commits.md and organize them into sections:
   - **✨ Features** (`feat:` commits)
   - **🐛 Bug Fixes** (`fix:` commits)
   - **📚 Documentation** (`docs:` commits)
   - **♻️ Refactoring** (`refactor:` commits)
   - **⚡ Performance** (`perf:` commits)
   - **🧪 Tests** (`test:` commits)
   - **🏗️ Build** (`build:` commits)
   - **🤖 CI/CD** (`ci:` commits)
   - **🔧 Chores** (`chore:` commits)
   - **⏪ Reverts** (`revert:` commits)
   - **🎨 Style** (`style:` commits)

4. **Create comprehensive PR description** with the following structure:
   ```markdown
   ## 📋 Summary
   [Brief 2-3 sentence overview of what this PR accomplishes]

   ## 🎯 Changes
   [Detailed list of changes organized by category]

   ### ✨ Features
   - [Feature 1 description]
   - [Feature 2 description]

   ### 🐛 Bug Fixes
   - [Fix 1 description]
   - [Fix 2 description]

   [Continue with other categories as applicable]

   ## 📊 Impact
   - **Modules Affected**: [List affected modules: core, api, websockets, streams, stepfunctions]
   - **Breaking Changes**: [Yes/No - if yes, describe]
   - **Migration Required**: [Yes/No - if yes, describe steps]

   ## 🧪 Testing
   - [ ] Unit tests pass
   - [ ] Integration tests pass (if applicable)
   - [ ] Manual testing completed
   - [ ] Code formatted with Spotless
   - [ ] SAM template validated

   ## 📝 Additional Notes
   [Any additional context, related issues, or follow-up work needed]

   ## 🔗 Related
   - Related Issues: [Issue numbers if applicable]
   - Related PRs: [PR numbers if applicable]
   ```

5. **Extract commit details**: For each commit, extract:
   - The emoji and type (feat, fix, docs, etc.)
   - The scope (if present)
   - The description
   - The body (if present) for additional context
   - Any issue references (Closes #123, Fixes #456, etc.)

6. **Get file changes summary**: Run `git diff --stat main..HEAD` to include a summary of files changed.

7. **Create the PR**: Use GitHub CLI to create the pull request:
   ```bash
   gh pr create --base main --head <current-branch> --title "<PR Title>" --body "<PR Description>"
   ```

   The PR title should be derived from the most significant change or the branch name if it follows a ticket format (e.g., "P2D-2129: Param Store Setters").

8. **If GitHub CLI is not authenticated**: Provide instructions for the user to authenticate or create the PR manually via GitHub web interface, including the generated description.

**Important Notes**:
- Always include all commits since the base branch
- Group related changes together
- Be specific about what changed and why
- Include breaking changes prominently if any
- Reference related issues/PRs when present
- Use emojis consistently with the commit message format
- If commits don't follow conventional format, still categorize them appropriately
- Include a testing checklist
- Mention any deployment considerations or environment-specific changes
