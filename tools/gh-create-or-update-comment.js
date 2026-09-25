const fs = require('fs');

module.exports = async ({github, context, bodyPath, bodyIncludes}) => {
    const body = fs.readFileSync(bodyPath, 'utf8');

    // Find existing comment
    const { data: comments } = await github.rest.issues.listComments({
        owner: context.repo.owner,
        repo: context.repo.repo,
        issue_number: context.issue.number
    });

    const botComment = comments.find(comment =>
        comment.user.login === 'github-actions[bot]' &&
        comment.body.includes(bodyIncludes)
    );

    // Create or update comment
    if (botComment) {
        await github.rest.issues.updateComment({
            owner: context.repo.owner,
            repo: context.repo.repo,
            comment_id: botComment.id,
            body: body
        });
        console.log(`Updated comment ${botComment.id}`);
    } else {
        const { data: newComment } = await github.rest.issues.createComment({
            owner: context.repo.owner,
            repo: context.repo.repo,
            issue_number: context.issue.number,
            body: body
        });
        console.log(`Created comment ${newComment.id}`);
    }
};
